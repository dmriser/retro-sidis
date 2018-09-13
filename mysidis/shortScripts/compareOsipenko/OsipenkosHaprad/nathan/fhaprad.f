      subroutine fhaprad(Ebeam,xc,q2c,zc,ptc,phic,sib,sig,delta,tail)
      implicit none
      double precision Ebeam,epsphir,epstau,epsrr,x,y,z,pt,
     &phi_had,xc,q2c,zc,ptc,phic,rcfac
      double precision sib,sig,delinf,delta,tai(3),tail
      double precision mp,mpi,mn,Sx,nu,Mx2,raddeg
      integer ivec,iphi_rad
      data mp/0.93827d0/,mpi/0.13957d0/,mn/0.93956536d0/,
     &raddeg/57.2957795131d0/
c Init
      sib=0.d0
      sig=0.d0
      delta=0.d0
      tail=0.d0
     
     
c Input for HAPRAD
      ivec=1       ! ivec - registered hadron: 1 - pi, 2 - K
      iphi_rad=0   ! iphi_rad - integration over phi_{rad} (1) or approximation (0)
      epsphir=0.01 ! relative accuracies of integration over phi_r,tau,R
      epstau=0.01
      epsrr=0.01
      
      x=xc
      y=-q2c       ! y or -Q2 if negative
      z=zc
      pt=ptc       ! pt or t if negative
      phi_had=phic/raddeg
      
      nu=q2c/(2.d0*mp*xc)
      Sx=2.d0*mp*nu
      Mx2=mp**2+Sx*(1.d0-zc)+ptc
      
      if(Mx2.gt.((mn+mpi)**2)) then
      call ihaprad(Ebeam,ivec,
     &iphi_rad,epsphir,epstau,epsrr,
     &x,y,z,pt,phi_had,sib,sig,delinf,delta,tai)
      rcfac=sig/sib
      sib=sib*1.d-3
      sig=sig*1.d-3
      tail=tai(2)*1.d-3
      else
      sib=0.d0
      sig=0.d0
      delta=0.d0
      tail=0.d0
      endif
      
      return
      end
