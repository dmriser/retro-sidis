      subroutine h3h4(Ebeam,x,q2,z,v,cos_phi,cos_2phi)
      implicit none
      double precision Ebeam,x,q2,z,v,cos_phi,cos_2phi
      double precision eps,gamma,kappa,zeta,h1oh2,h2oh1h2,y,nu
      double precision I1,I2,H1,H2,H3,H4,a2,b2,rlt,mp
      double precision cos_cahn,cos_berger,cos2_cahn,cos2_berger
      data mp/0.93827d0/,rlt/0.12d0/
      data a2/0.25d0/,b2/0.20d0/
      cos_phi=0.d0
      cos_2phi=0.d0
c Kinematics
      nu=q2/(2.d0*mp*x)
      y=nu/Ebeam
      if((1.d0-y-mp*x*y/(2.d0*Ebeam)).lt.0.d0) return
      eps=x*y**2/(1.d0-y-mp*x*y/(2.d0*Ebeam))
      gamma=2.d0*mp*x/sqrt(q2)
      kappa=1.d0/(1.d0+gamma**2)
      zeta=1.d0-y-gamma**2*y**2/4.d0
      h1oh2=1.d0/(2.d0*x*kappa)*1.d0/(1.d0+rlt)
c      h2oh1h2=1.d0/(1.d0+eps*h1oh2)
c Approximate
      h2oh1h2=(1.d0-y)/(1.d0+(1.d0-y)**2)
c Cahn effect
      cos_cahn=-sqrt(v)*2.d0*a2*z/(b2+a2*z**2)*h2oh1h2
      cos2_cahn=v*2.d0*z**2*a2**2/((b2+a2*z**2)**2)*h2oh1h2
c Structure functions
      H1=(I2(z,v)**2+v**2*I1(z,v)**2)/(2.d0*x)
      H2=I2(z,v)**2+4.d0*v*z**2*I1(z,v)**2+v**2*I1(z,v)**2
      H3=z*I1(z,v)*(I2(z,v)-v*I1(z,v))
      H4=-I1(z,v)*I2(z,v)
c Berger effect
      cos_berger=sqrt(v)*H3/(H2+eps*H1)
c Ad Hoc correction to describe CLAS data
     &*3.d0*(1.d0+2.d0*z**2/sqrt(0.1d0+v))
      cos2_berger=v*H4/(H2+eps*H1)
c Total contribution
      cos_phi=cos_cahn+cos_berger
      cos_2phi=cos2_cahn+cos2_berger
c From H3,4/(H2+eps*H1) to cosines
      cos_phi=(2.d0-y)*sqrt(kappa/zeta)*cos_phi
      cos_2phi=kappa*cos_2phi
      return
      end

      double precision function I1(z,v)
      implicit none
      double precision z,v,ximin,ximax,prec,DGAUSS,I1_integ,zm,vm
      double precision pi,DGamma,Hypergeometric2F1
      data ximin/0.d0/,ximax/1.d0/,prec/1.d-8/
      data pi/3.14159265359d0/
      common/vrs/zm,vm
      external I1_integ
      zm=z
      vm=v
c      I1=z*DGAUSS(I1_integ,ximin,ximax,prec)
c Analytic integration
      I1=1.d0/0.529992d0*
     &sqrt(pi)*DGamma(4.d0/3.d0)*Hypergeometric2F1(z-v/z)/
     &(2.d0*2.d0**(2.d0/3.d0)*DGamma(11.d0/6.d0))
c      print*,I1,I1m,(z-v/z)
c      stop
      return
      end

      double precision function I2(z,v)
      implicit none
      double precision z,v,I1
      I2=5.d0-z**2*I1(z,v)
      return
      end

      double precision function I1_integ(xi)
      implicit none
      double precision xi,phi_b,z,v
      common/vrs/z,v
      I1_integ=phi_b(xi)/(z-xi*(z**2-v))
      return
      end

      double precision function phi_b(xi)
      implicit none
      double precision xi,betab
      data betab/0.529992d0/
      phi_b=((1.d0-xi)*xi)**(1.d0/3.d0)/betab
      return
      end

      double precision function Hypergeometric2F1(z)
c Hypergeometric function 2F1(1,4/3,8/3,z)
      implicit none
      integer np,pd
      parameter (np=51)
      double precision z
      real zv(np),h2f1(np),DIVDIF
      data zv/-1370.0,-1200.0,-1100.0,-1000.0,-900.0,-800.0,-700.0,
     &-600.0,-500.0,-400.0,-300.0,-200.0,-100.0,
     &-90.0,-80.0,-70.0,-60.0,-50.0,-40.0,-30.0,
     &-20.0,-10.0,-9.0,-8.0,-7.0,-6.0,-5.0,
     &-4.0,-3.0,-2.0,-1.0,-0.9,-0.8,-0.7,
     &-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,
     &0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,
     &0.9,1.0/
      data h2f1/0.00320235,0.00363324,0.00394662,0.00432031,0.00477374,0.00533578,0.00605128,
     &0.00699393,0.0082941,0.0102069,0.0133102,0.0192631,0.0357037,
     &0.0391384,0.0433407,0.0486079,0.0554171,0.0645876,0.0776615,0.0979519,
     &0.134232,0.221092,0.237297,0.256366,0.279181,0.307049,0.341996,
     &0.387364,0.449153,0.539504,0.688151,0.708757,0.730914,0.754825,
     &0.780729,0.808917,0.839741,0.873637,0.911149,0.952971,1.0,
     &1.05343,1.11488,1.18663,1.27204,1.3763,1.50805,1.68328,1.93682,
     &2.37297,5.0/
      data pd/2/
      Hypergeometric2F1=0.d0
      if(z.gt.-1370.d0.and.z.lt.1.d0) then
      Hypergeometric2F1=dble(DIVDIF(h2f1,zv,np,real(z),pd))
      else
      print*,'WARNING Hypergeometric2F1 OUT OF RANGE z= ',z
      endif
      return
      end
