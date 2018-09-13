      subroutine semi_inclusive_model(q2,x,y,z,pt2,mx2,pl,H1z,H2z,H3z,H4z)
c Semi-inclusive cross section from the product
c of parton distribution and fragmentation functions
      implicit none
      include 'partons.inc'
      include 'constants8.inc'
      double precision q2,x,y,z,pt2,mx2,pl
      double precision XD,ZD,Q2D
      double precision H1z,H2z,H3z,H4z
      double precision SCALE,UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL
      double precision uff(2),dff(2),sff(2),cff(2),bff(2),gff
      double precision GTMD,H1,H2,H3,H4,nu,Eh,ph2,zmin
      double precision igev2mb,rlt
      double precision uq,dq,sq,cq,bq,tq,gg
      double precision qm,pi_thresh,r,xi,Ebeam,cterm,cos_phi,cos_2phi
      double precision mh,w2,pt2_mean,pt2_max,pt2_mean_he,eps,gamma,kappa,zeta
      integer nc,GPDF,SPDF,IFINI,ISET,ICHARGE
      data rlt/0.12d0/
      COMMON/FRAGINI/IFINI
c NLO PDFs
c      data GPDF/5/,SPDF/7/  ! GRV 94 HO NLL DIS
c      data GPDF/3/,SPDF/36/ ! MRS H NLL DIS
c      data GPDF/4/,SPDF/28/ ! CTEQ 2pD NLL DIS
c      data GPDF/2/,SPDF/8/  ! DFLM 260 NLL DIS
c LO PDFs
      data GPDF/5/,SPDF/5/  ! GRV 94 LO
c      data GPDF/3/,SPDF/72/ ! MRST c-g LO
c      data GPDF/4/,SPDF/32/ ! CTEQ 4L LO
c      data GPDF/2/,SPDF/5/  ! DFLM ca LO
c Pi+ FF
      data ISET/1/,ICHARGE/1/
      data nc/0/
      data igev2mb/389.379292d0/
c Init
      UPV=0.d0
      DNV=0.d0
      USEA=0.d0
      DSEA=0.d0
      STR=0.d0
      CHM=0.d0
      BOT=0.d0
      TOP=0.d0
      GL=0.d0
      uff(1)=0.d0
      dff(1)=0.d0
      sff(1)=0.d0
      cff(1)=0.d0
      bff(1)=0.d0
      uff(2)=0.d0
      dff(2)=0.d0
      sff(2)=0.d0
      cff(2)=0.d0
      bff(2)=0.d0
      gff=0.d0
      H1=0.d+0
      H2=0.d+0
      H3=0.d+0
      H4=0.d+0
      H1z=0.d+0
      H2z=0.d+0
      H3z=0.d+0
      H4z=0.d+0
c Check kinematics
      if(x.le.0.d0.or.x.ge.1.d0) return
      if(z.le.0.d0.or.z.ge.1.d0) return
      nc=nc+1
c      print*,nc
c Obtain Parton Distribution Functions
      XD=x
c      r=sqrt(1.d0+(2.d0*mp*x)**2/q2)
c      xi=2.d0*x/(1.d0+r)
c      XD=xi
c QCD scale
      if(q2.gt.1.d0) then
      SCALE=sqrt(q2)
      else
      SCALE=1.0000001
      endif
c Init PDFs
      if(nc.eq.1) call init_pdf(GPDF,SPDF)
      call STRUCTM(XD,SCALE,UPV,DNV,USEA,DSEA,STR,CHM,BOT,TOP,GL)
c Obtain Parton Fragmentation Functions
      if(z.gt.0.01) then
      ZD=z
      else
      ZD=0.01000001
      endif
      if(q2.gt.1.d0) then
      Q2D=q2
      else
      Q2D=1.0000001
      endif
c Init FFs
      IFINI=1
      if(nc.eq.1) IFINI=0
      call PKHFF(ISET,ICHARGE,ZD,Q2D,uff,dff,sff,cff,bff,gff)
c Kinematics
      mh=mpi
      nu=q2/(2.d0*mp*x)
      w2=mp**2+2.d0*mp*nu-q2
c      qm=sqrt(q2+nu**2)
      Ebeam=nu/y
c      if(y.lt.0.d0.or.y.gt.1.d0) return
      Eh=z*nu
c      if(Eh.lt.mh) return
      ph2=max(0.d0,(Eh**2-mh**2))
c      if(ph2.lt.pt2) return
      if(mx2.lt.((mp+mpi)**2)) return
c      pi_thresh=sqrt(1.d0-(mp+mpi)**2/mx2)
c      pi_thresh=(1.d0-(mp+mpi)**2/mx2)**(0.2d0/q2)
c Model 2
c      pi_thresh=(1.d0-(mp+mpi)**2/mx2)**(4.d0*mp**2*x**2/q2)
c Model 3
      pi_thresh=(1.d0-(mp+mpi)**2/mx2)**0.05d0
c Parton transverse momentum distribution (Gaussian fit)
c      pt2_mean=0.25d0*z**2+0.31d0
c Model 1
c      pt2_mean=0.0175d0*w2*(1.d0+4.d0*(1.d0-z)**2)
c Model 2,3
c      pt2_mean=sqrt(w2)*0.12d0*sqrt(z)
c Model 4
      zmin=mh/nu
      pt2_mean=sqrt(w2)*0.12d0*sqrt(z)/(1.d0+(3.2d0*zmin/z)**4)
c Low-z correction for maximum allowed pt2
      pt2_max=ph2
      pt2_mean=pt2_mean/(1.d0+pt2_mean/max(0.001d0,pt2_max))
      if(pl.gt.0.d0) then
      GTMD=exp(-pt2/pt2_mean)/((pi*pt2_mean)*(1.d0-exp(-pt2_max/pt2_mean)))
      else
      GTMD=exp(-(pl**2+ph2)/pt2_mean)/((pi*pt2_mean)*(1.d0-exp(-pt2_max/pt2_mean)))
      endif
c z-distribution correction
      pt2_mean_he=0.25d0*z**2+0.31d0
      GTMD=GTMD*(1.d0-exp(-pt2_max/pt2_mean_he))/0.7d0
c Semi-inclusive structure functions at LO and LT
      uq=eu2*((UPV+USEA)*uff(1)+USEA*uff(2))
      dq=ed2*((DNV+DSEA)*dff(1)+DSEA*dff(2))
      sq=es2*(STR*sff(1)+STR*sff(2))
      cq=ec2*(CHM*cff(1)+CHM*cff(2))
      bq=eb2*(BOT*bff(1)+BOT*bff(2))
      tq=0.0d+0
      gg=0.0d+0
      H2=(uq+dq+sq+cq+bq+tq+gg)*GTMD*pi_thresh
      H1=H2/(2.d0*x*(1.d0+rlt))*(1.d0+4.d0*mp**2*x**2/q2)
c H3 and H4
      eps=x*y**2/(1.d0-y-mp*x*y/(2.d0*Ebeam))
      gamma=2.d0*mp*x/sqrt(q2)
      kappa=1.d0/(1.d0+gamma**2)
      zeta=1.d0-y-gamma**2*y**2/4.d0
      call h3h4(Ebeam,x,q2,z,(pt2/q2),cos_phi,cos_2phi)
      cterm=eps*H1+H2
c Check positivity of cross section
      call positivity_cos(cos_phi,cos_2phi)
c Extract H3 and H4
      H3=(cos_phi*sqrt(zeta/kappa)/(2.d0-y))*2.d0*cterm
      H4=(cos_2phi/kappa)*2.d0*cterm
c Keep in hadronic tensor of Levlet & Mulders, PRD49, 96 (1994)
      H1z=H1
      H2z=H2
      H3z=H3
      H4z=H4
c Convert into new hadronic tensor of Akushevich, Eur.Phys.J. C10, 681 (1999)
c      aa=(Eh*qm-ph_long*nu)/(mp*qm)
c      H1z=2.d0*z/mp*(H1-pt**2/(q2*2.d0*x)*H4)
c      H2z=2.d0*z/(nu*mp**2)*(H2+aa/x*H3+
c     &nu**2/q2*(2.d0*aa**2*mp**2/q2-
c     &(pt/qm)**2)*H4)
c      H3z=4.d0*z*nu/(q2**2)*H4
c      H4z=-2.0*z/(mp*q2)*(H3+aa/x*H4)

c      print*,'new kinem: ',q2,x,z,pt
c      print*,H1z,H2z,H3z,H4z,GTMD,pi_thresh,UPV,uff(1)

      return
      end
