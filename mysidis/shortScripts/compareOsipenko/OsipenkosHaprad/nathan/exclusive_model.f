      subroutine exclusive_model(q2m,wm,csthcm,st,sl,stt,stl,stlp)
      implicit none
      double precision st,sl,stt,stl,stlp
      integer nq,nw,nt,iq,iw,it,nc,narg(3)
      parameter(nq=18)
      parameter(nw=47)
      parameter(nt=61)
      double precision q2,w,csthcm,dfint,th_cm,degrad,
     &q2_pn(nq),w_pn(nw),th_cm_pn(nt),
     &ft_cs(nq,nw,nt),fl_cs(nq,nw,nt),ftt_cs(nq,nw,nt),
     &ftl_cs(nq,nw,nt),ftlp_cs(nq,nw,nt),
     &arg(3),rarg(1000),a2,a30,a31,a3,wcor,q2cor
      double precision q2m,wm
      data q2_pn/0.0d0,0.3d0,0.6d0,0.9d0,1.2d0,1.5d0,1.8d0,2.1d0,
     &2.4d0,2.7d0,3.0d0,3.3d0,3.6d0,3.9d0,4.2d0,4.5d0,4.8d0,5.0d0/,
     &w_pn/1.08d0,1.10d0,1.12d0,1.14d0,1.16d0,1.18d0,1.20d0,1.22d0,
     &1.24d0,1.26d0,1.28d0,1.30d0,1.32d0,1.34d0,1.36d0,1.38d0,1.40d0,
     &1.42d0,1.44d0,1.46d0,1.48d0,1.50d0,1.52d0,1.54d0,1.56d0,1.58d0,
     &1.60d0,1.62d0,1.64d0,1.66d0,1.68d0,1.70d0,1.72d0,1.74d0,1.76d0,
     &1.78d0,1.80d0,1.82d0,1.84d0,1.86d0,1.88d0,1.90d0,1.92d0,1.94d0,
     &1.96d0,1.98d0,2.00d0/,
     &th_cm_pn/0.d0,3.d0,6.d0,9.d0,12.d0,15.d0,18.d0,21.d0,24.d0,27.d0,30.d0,
     &33.d0,36.d0,39.d0,42.d0,45.d0,48.d0,51.d0,54.d0,57.d0,60.d0,63.d0,66.d0,
     &69.d0,72.d0,75.d0,78.d0,81.d0,84.d0,87.d0,90.d0,93.d0,96.d0,99.d0,102.d0,
     &105.d0,108.d0,111.d0,114.d0,117.d0,120.d0,123.d0,126.d0,129.d0,132.d0,
     &135.d0,138.d0,141.d0,144.d0,147.d0,150.d0,153.d0,156.d0,159.d0,162.d0,
     &165.d0,168.d0,171.d0,174.d0,177.d0,180.d0/,
     &nc/0/,narg/nq,nw,nt/,degrad/57.29577952d0/,
     &a2/1.15d0/,a30/-1.23d0/,a31/0.16d0/
      common/exlusive/rarg,ft_cs,fl_cs,ftt_cs,ftl_cs,ftlp_cs
c Init
      st=0.d0
      sl=0.d0
      stt=0.d0
      stl=0.d0
      stlp=0.d0
c new variables
      q2=q2m
      w=wm
      th_cm=acos(csthcm)*degrad
c Check kinematics
      if(abs(csthcm).gt.1.d0) return
      if(w.lt.1.07784) return
      if(q2.lt.0.d0) then
      print*,'Warning: Q2<0 in exclusive model!'
      print*,'Using Q2=0'
      q2=0.d0
      endif
c Correction on high-w slope (W>2 GeV MAID is constant) (A.Browman PRL35,Cornell)
      if(w.gt.2.d0) then
c      a3=a30+a31*th_cm
c      if(th_cm.lt.50.d0) a3=a30+a31*50.d0 ! This is wrong: slope goes to zero at th_cm=0
c      if(th_cm.gt.100.d0) a3=a30+a31*100.d0
c New slope: goes to zero at th_cm=0 and close to Cornell points
      a3=0.002*th_cm**2
      if(a3.gt.12.d0) a3=12.d0
      wcor=(2.d0**a3)/(w**a3)
      w=2.d0
      else
      wcor=1.d0
      endif
c Correction on high-Q2 slope (Q2>5 GeV2 MAID is constant) (A.Browman PRL35,Cornell)
      if(q2.gt.5.d0) then
      q2cor=(5.d0**a2)/(q2**a2)
      q2=5.d0
      else
      q2cor=1.d0
      endif
c Read data from file
      if(nc.eq.0) then
      open(44,file='pi_n_maid.dat',status='old')
       do iq=1,nq
        do iw=1,nw
         do it=1,nt
      read(44,*) ft_cs(iq,iw,it),fl_cs(iq,iw,it),
     &ftt_cs(iq,iw,it),ftl_cs(iq,iw,it),ftlp_cs(iq,iw,it)
         enddo
        enddo
       enddo
      close(44)
       do iq=1,nq
	rarg(iq)=q2_pn(iq)
       enddo
       do iw=1,nw
	rarg(iw+nq)=w_pn(iw)
       enddo
       do it=1,nt
	rarg(it+nw+nq)=th_cm_pn(it)
       enddo
	nc=nc+1
      endif
c Extract interpolated cross section
c      if(q2.ge.q2_pn(1).and.w.ge.w_pn(1).and.th_cm.ge.th_cm_pn(1)
c     &.and.q2.le.q2_pn(nq).and.w.le.w_pn(nw)
c     &.and.th_cm.le.th_cm_pn(nt)) then

	arg(1)=q2
	arg(2)=w
	arg(3)=th_cm

      st=dfint(3,arg,narg,rarg,ft_cs)*wcor*q2cor
      sl=dfint(3,arg,narg,rarg,fl_cs)*wcor*q2cor
      stt=dfint(3,arg,narg,rarg,ftt_cs)*wcor*q2cor
      stl=2.d0*dfint(3,arg,narg,rarg,ftl_cs)*wcor*q2cor
      stlp=0.d0
c      stlp=dfint(3,arg,narg,rarg,ftlp_cs)*wcor*q2cor

c	else
c      st=0d0
c      sl=0d0
c      stt=0d0
c      stl=0d0
c      stlp=0d0
c	endif
	return
	end
