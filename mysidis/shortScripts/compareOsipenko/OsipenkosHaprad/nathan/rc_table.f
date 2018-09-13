      program rc_table
      implicit none
      double precision Ebeam,x,q2,z,t_a,t_b,phi,nu
      double precision sib,sig,delta,tail,rcb
      double precision pi,mp,mpi,rc,s,Eh,ph,qv
      double precision ph2,pt2,pl2,pl
      double precision phi_min,phi_max,t_min,t_max,z_min,z_max
      double precision x_min,x_max
      double precision sib_a,sig_a,delta_a,tail_a,sib_b,sig_b,delta_b,tail_b
      integer iphi
      data pi/3.14159265d0/,mpi/0.1395675d0/,mp/0.938272d0/,
     &Ebeam/5.754d0/
c Init
      s=2.d0*mp*Ebeam
      phi_min=0.d0
      phi_max=360.d0
c Output file
      open(36,file='rc_table.dat',status='unknown')
c Kinematics
      q2=2.020e0
      x_min=q2/(2.d0*mp*Ebeam)
      x_max=q2/(q2+(mp+mpi)**2-mp**2)
      
      x=0.26861545
      nu=q2/(2.d0*mp*x)
      z_min=mpi/nu
      z_max=1.d0
c Integrate over pion
      z=9.32860672E-02
      Eh=z*nu
      qv=sqrt(nu**2+q2)
      ph2=Eh**2-mpi**2
      
      if(ph2.gt.0.d0) then
      ph=sqrt(ph2)
      else
      ph=0.d0
      endif
      
      if(Eh.gt.mpi) then
      t_min=-q2+mpi**2-2.d0*(nu*Eh+qv*ph)
      if(abs(t_min).gt.s) t_min=-s
      t_max=-q2+mpi**2-2.d0*(nu*Eh-qv*ph)
      else
      t_min=0.d0
      t_max=0.d0
      endif
      
c      print*,'tmin-tmax: ',t_min,t_max

      pt2=0.1609e0*0.1609e0
      pl2=ph2-pt2

      if(pl2.gt.0.d0) then
      pl=sqrt(pl2)
      else
      pl=0.d0
      endif
      
      t_a=mpi**2-q2-2.d0*nu**2*z+2.d0*pl*qv
      t_b=mpi**2-q2-2.d0*nu**2*z-2.d0*pl*qv
      
      do iphi=1,18
      phi=(dble(iphi)-0.5)*360.e0/18.e0
      
      print*,'start---->',x,z,t_a,t_b,phi
      
      if(x.gt.x_min.and.x.lt.x_max
     &.and.z.gt.z_min.and.z.lt.z_max
     &.and.ph.gt.0.d0.and.pl.gt.0.d0.and.t_max.gt.t_min) then
      
      if(t_a.gt.t_min.and.t_a.lt.t_max) then
      call fhaprad(Ebeam,x,q2,z,t_a,phi,sib_a,sig_a,delta_a,tail_a)
      else
      sib_a=0.d0
      sig_a=0.d0
      delta_a=0.d0
      tail_a=0.d0
      endif
      
      if(t_b.gt.t_min.and.t_b.lt.t_max) then
      call fhaprad(Ebeam,x,q2,z,t_b,phi,sib_b,sig_b,delta_b,tail_b)
      else
      sib_b=0.d0
      sig_b=0.d0
      delta_b=0.d0
      tail_b=0.d0
      endif
      
      sib=sib_a+sib_b
      sig=sig_a+sig_b
      delta=delta_a+delta_b
      tail=tail_a+tail_b
      
c      print*,'Result!'
c      print*,x,z,t,phi,sib,sig,delta,tail
c      stop
      
      if(sib.lt.1.d-99.or.sib.gt.1.d+99) sib=0.d0
      if(sig.lt.1.d-99.or.sig.gt.1.d+99) sig=0.d0
      if(abs(delta).lt.(1.d-99).or.abs(delta).gt.(1.d+99)) delta=0.d0
      if(tail.lt.1.d-99.or.tail.gt.1.d+99) tail=0.d0
      
c      print*,'----Result----'
c      print*,sib,sig,delta,tail
c      stop
      
      else
      sib=0.d0
      sig=0.d0
      delta=0.d0
      tail=0.d0
      endif
      
      if(sib.gt.0.d0) then
      rc=sig/sib
      rcb=(sig-tail)/sib
      else
      rc=0.d0
      rcb=0.d0
      endif
      
      
      write(36,20) rc
      
      
      enddo
      
      close(36)
10    format(1pe11.4$)
20    format(1pe11.4)
      end
