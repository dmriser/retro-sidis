      subroutine simpu(a1,b1,h1,reps1,aeps1,funct,x,ai,aih,aiabs)
      implicit double precision(a-h,o-z)
      dimension f(7),p(5)
      h=sign(h1,b1-a1)
      s=sign(1.d0,h)
      a=a1
      b=b1
      ai=0.d0
      aih=0.d0
      aiabs=0.d0
      p(2)=4.d0
      p(4)=4.d0
      p(3)=2.d0
      p(5)=1.d0
      if(b-a) 1,2,1
    1 reps=abs(reps1)
      aeps=abs(aeps1)
      do 3 k=1,7
  3   f(k)=10.d16
      x=a
      c=0.d0
      f(1)=funct(x)/3.d0
      
c      print*,'c ',x,f(1)
      
    4 x0=x
      if((x0+4.d0*h-b)*s) 5,5,6
    6 h=(b-x0)/4.d0
      if(h) 7,2,7
    7 do 8 k=2,7
  8   f(k)=10.d16
      c=1.d0
    5 di2=f(1)
      di3=abs(f(1))
      do 9 k=2,5
      x=x+h
      if((x-b)*s) 23,24,24
   24 x=b
   23 if(f(k)-10.d16) 10,11,10
   11 f(k)=funct(x)/3.d0
      
c      print*,'d'
c      write(*,'(2f19.15)') x,f(k)
      
   10 di2=di2+p(k)*f(k)
    9 di3=di3+p(k)*abs(f(k))
      di1=(f(1)+4.d0*f(3)+f(5))*2.d0*h
      di2=di2*h
      di3=di3*h
      if(reps) 12,13,12
   13 if(aeps) 12,14,12
   12 eps=abs((aiabs+di3)*reps)
      if(eps-aeps) 15,16,16
   15 eps=aeps
   16 delta=abs(di2-di1)
      if(delta-eps) 20,21,21
   20 if(delta-eps/8.d0) 17,14,14
   17 h=2.d0*h
      f(1)=f(5)
      f(2)=f(6)
      f(3)=f(7)
      do 19 k=4,7
  19  f(k)=10.d16
      go to 18
   14 f(1)=f(5)
      f(3)=f(6)
      f(5)=f(7)
      f(2)=10.d16
      f(4)=10.d16
      f(6)=10.d16
      f(7)=10.d16
   18 di1=di2+(di2-di1)/15.d0
      ai=ai+di1
      aih=aih+di2
      aiabs=aiabs+di3
      go to 22
   21 h=h/2.d0
      f(7)=f(5)
      f(6)=f(4)
      f(5)=f(3)
      f(3)=f(2)
      f(2)=10.d16
      f(4)=10.d16
      x=x0
      c=0.d0
      go to 5
   22 if(c) 2,4,2
    2 return
      end

CDECK  ID>, SIMPS.
      subroutine simpsz(a1,b1,h1,reps1,aeps1,funct,x,ai,aih,aiabs)
c simps
c a1,b1 -the limits of integration
c h1 -an initial step of integration
c reps1,aeps1 - relative and absolute precision of integration
c funct -a name of function subprogram for calculation of integrand +
c x - an argument of the integrand
c ai - the value of integral
c aih- the value of integral with the step of integration
c aiabs- the value of integral for module of the integrand
c this subrogram calculates the definite integral with the relative or
c absolute precision by simpson+s method with the automatical choice
c of the step of integration
c if aeps1    is very small(like 1.e-17),then calculation of integral
c with reps1,and if reps1 is very small (like 1.e-10),then calculation
c of integral with aeps1
c when aeps1=reps1=0. then calculation with the constant step h1
c
      implicit double precision(a-h,o-z)
      dimension f(7),p(5)
      h=sign(h1,b1-a1)
      s=sign(1.d0,h)
      a=a1
      b=b1
      ai=0.d0
      aih=0.d0
      aiabs=0.d0
      p(2)=4.d0
      p(4)=4.d0
      p(3)=2.d0
      p(5)=1.d0
      if(b-a) 1,2,1
    1 reps=abs(reps1)
      aeps=abs(aeps1)
      do 3 k=1,7
  3   f(k)=10.d16
      x=a
      c=0.d0
      f(1)=funct(x)/3.d0
    4 x0=x
      if((x0+4.d0*h-b)*s) 5,5,6
    6 h=(b-x0)/4.d0
      if(h) 7,2,7
    7 do 8 k=2,7
  8   f(k)=10.d16
      c=1.d0
    5 di2=f(1)
      di3=abs(f(1))
      do 9 k=2,5
      x=x+h
      if((x-b)*s) 23,24,24
   24 x=b
   23 if(f(k)-10.d16) 10,11,10
   11 f(k)=funct(x)/3.d0
   10 di2=di2+p(k)*f(k)
    9 di3=di3+p(k)*abs(f(k))
      di1=(f(1)+4.d0*f(3)+f(5))*2.d0*h
      di2=di2*h
      di3=di3*h
      if(reps) 12,13,12
   13 if(aeps) 12,14,12
   12 eps=abs((aiabs+di3)*reps)
      if(eps-aeps) 15,16,16
   15 eps=aeps
   16 delta=abs(di2-di1)
      if(delta-eps) 20,21,21
   20 if(delta-eps/8.d0) 17,14,14
   17 h=2.d0*h
      f(1)=f(5)
      f(2)=f(6)
      f(3)=f(7)
      do 19 k=4,7
  19  f(k)=10.d16
      go to 18
   14 f(1)=f(5)
      f(3)=f(6)
      f(5)=f(7)
      f(2)=10.d16
      f(4)=10.d16
      f(6)=10.d16
      f(7)=10.d16
   18 di1=di2+(di2-di1)/15.d0
      ai=ai+di1
      aih=aih+di2
      aiabs=aiabs+di3
      go to 22
   21 h=h/2.d0
      f(7)=f(5)
      f(6)=f(4)
      f(5)=f(3)
      f(3)=f(2)
      f(2)=10.d16
      f(4)=10.d16
      x=x0
      c=0.d0
      go to 5
   22 if(c) 2,4,2
    2 return
      end

      subroutine simpsx(a,b,np,ep,func,res)
      implicit double precision (a-h,o-z)
      external func
      step=(b-a)/dble(np)
      call simpsz(a,b,step,ep,1d-18,func,ra,res,r2,r3)
      return
      end

      subroutine simptx(a,b,np,ep,func,res)
      implicit double precision (a-h,o-z)
      external func
      step=(b-a)/dble(np)
      call simpt(a,b,step,ep,1d-18,func,ra,res,r2,r3)
      return
      end

      subroutine simpux(a,b,np,ep,func,res)
      implicit double precision (a-h,o-z)
      external func
      step=(b-a)/dble(np)
      call simpu(a,b,step,ep,1d-18,func,ra,res,r2,r3)
      return
      end

      subroutine simpt(a1,b1,h1,reps1,aeps1,funct,x,ai,aih,aiabs)
      implicit double precision(a-h,o-z)
      dimension f(7),p(5)
      h=sign(h1,b1-a1)
      s=sign(1.d0,h)
      a=a1
      b=b1
      ai=0.d0
      aih=0.d0
      aiabs=0.d0
      p(2)=4.d0
      p(4)=4.d0
      p(3)=2.d0
      p(5)=1.d0
      if(b-a) 1,2,1
    1 reps=abs(reps1)
      aeps=abs(aeps1)
      do 3 k=1,7
  3   f(k)=10.d16
      x=a
      c=0.d0
      f(1)=funct(x)/3.d0
      
c      print*,'a ',x,f(1)
      
    4 x0=x
      if((x0+4.d0*h-b)*s) 5,5,6
    6 h=(b-x0)/4.d0
      if(h) 7,2,7
    7 do 8 k=2,7
  8   f(k)=10.d16
      c=1.d0
    5 di2=f(1)
      di3=abs(f(1))
      do 9 k=2,5
      x=x+h
      if((x-b)*s) 23,24,24
   24 x=b
   23 if(f(k)-10.d16) 10,11,10
   11 f(k)=funct(x)/3.d0
      
c      print*,'b ',x,f(k)
      
      
   10 di2=di2+p(k)*f(k)
    9 di3=di3+p(k)*abs(f(k))
      di1=(f(1)+4.d0*f(3)+f(5))*2.d0*h
      di2=di2*h
      di3=di3*h
      if(reps) 12,13,12
   13 if(aeps) 12,14,12
   12 eps=abs((aiabs+di3)*reps)
      if(eps-aeps) 15,16,16
   15 eps=aeps
   16 delta=abs(di2-di1)
      if(delta-eps) 20,21,21
   20 if(delta-eps/8.d0) 17,14,14
   17 h=2.d0*h
      f(1)=f(5)
      f(2)=f(6)
      f(3)=f(7)
      do 19 k=4,7
  19  f(k)=10.d16
      go to 18
   14 f(1)=f(5)
      f(3)=f(6)
      f(5)=f(7)
      f(2)=10.d16
      f(4)=10.d16
      f(6)=10.d16
      f(7)=10.d16
   18 di1=di2+(di2-di1)/15.d0
      ai=ai+di1
      aih=aih+di2
      aiabs=aiabs+di3
      go to 22
   21 h=h/2.d0
      f(7)=f(5)
      f(6)=f(4)
      f(5)=f(3)
      f(3)=f(2)
      f(2)=10.d16
      f(4)=10.d16
      x=x0
      c=0.d0
      go to 5
   22 if(c) 2,4,2
    2 return
      end

CDECK  ID>, D01FCE.
      subroutine d01fce(ndim, a, b, minpts, maxpts, functn, eps,
     * acc, lenwrk, wrkstr, finval, ifail)
      implicit double precision(a-h,o-z)
c     mark 8 release. nag copyright 1979.
c
c     adaptive multidimensional integration subroutine
c
c     *********  parameters for d01fce ****************************
c
c      input parameters
c
c     ndim    integer number of variables, must exceed 1 but
c	  not exceed 15.
c
c     a       real array of lower limits, with dimension ndim
c
c     b       real array of upper limits, with dimension ndim
c
c     minpts  integer minimum number of integrand values to be
c	  allowed, which must not exceed maxpts.
c
c     maxpts  integer maximum number of integrand values to be
c	  allowed, which must be at least
c	  2**ndim+2*ndim**2+2*ndim+1.
c
c     functn  externally declared user defined real function
c	  integrand. it must have parameters (ndim,z),
c	  where z is a real array of dimension ndim.
c
c     eps     real required relative accuracy, must be greater
c	  than zero
c
c     lenwrk  integer length of array wrkstr, must be at least
c	  2*ndim+4.
c
c     ifail   integer nag failure parameter
c	  ifail=0 for hard fail
c	  ifail=1 for soft fail
c
c      output parameters
c
c     minpts  integer number of integrand values used by the
c	  routine
c
c     wrkstr  real array of working storage of dimension (lenwrk).
c
c     acc     real estimated relative accuracy of finval
c
c     finval  real estimated value of integral
c
c     ifail   ifail=0 for normal exit, when estimated relative
c	    less integaccuracy rand values used.
c
c      ifail=1 if ndim.lt.2, ndim.gt.15, minpts.gt.maxpts,
c	    maxpts.lt.2**ndim+2*ndim*(ndim+1)+1, eps.le.0
c	    or lenwrk.lt.2*ndim+4.
c
c      ifail=2 if maxpts was too small for d01fce to obtain the
c	    required relative accuracy eps.  in this
c	    case d01fce returns a value of finval
c	    with estimated relative accuracy acc.
c
c      ifail=3 if lenwrk too small for maxpts integrand
c	    values.  in this case d01fce returns a
c	    value of finval with estimated accuracy
c	    acc using the working storage
c	    available, but acc will be greater
c	    than eps.
c
c     **************************************************************
c
c     .. scalar arguments ..
      double precision eps, finval, acc
      integer ifail, lenwrk, maxpts, minpts, ndim
c     .. array arguments ..
      double precision a, b, wrkstr
      dimension a(ndim), b(ndim), wrkstr(lenwrk)
c     .. function arguments ..
      double precision functn
c     ..
c     .. local scalars ..
      character*8 srname
      double precision 
     * abserr, df1, df2, difmax, f1, f2, f3, f4, half, lamda2,
     * lamda4, lamda5, one, ratio, rgncmp, rgnerr, rgnert, rgnval,
     * rgnvlt, rgnvol, rlndim, sum1, sum2, sum3, sum4, sum5, two,
     * twondm, weit1, weit2, weit3, weit4, weit5, weitp1, weitp2,
     * weitp3, weitp4, zero
      integer dvaxes, dvaxis, dvflag, funcls, ierror, j, k, maxaxs,
     * mxrgns, pointr, rgncls, rulcls, sbrgns, subrgn, subtmp,
     * tpontp, tpontr
c     .. local arrays ..
      dimension center(15), dif(15), oldcnt(15), width(15), z(15)
      integer dvcntl(15), dvcntr(15)
c     .. function references ..
      double precision x02aae
      integer p01aae, x02bbe
c     ..
      data srname /'  d01fce'/
      data zero, one, two, half /0.d0, 1.d0, 2.d0, 0.5d0/
c
c   subroutine initialisation and parameter checking
c
      if (ndim.lt.2 .or. ndim.gt.15) go to 560
      if (minpts.gt.maxpts) go to 560
      if (eps.le.zero) go to 560
      if (lenwrk.lt.2*ndim+4) go to 560
      funcls = 0
      finval = zero
      abserr = zero
      twondm = two**ndim
      rgnvol = twondm
      dvflag = 1
      fffff1 = dble(x02bbe(one))
      fffff2 = 1.d0/x02aae(0.0d0)
      maxaxs = int(min(fffff1,fffff2))
c     maxaxs = int(amin1(float(x02bbe(one)),1.0/x02aae(0.0d0)))
      maxaxs = (maxaxs-ndim)/(ndim+1)
      mxrgns = lenwrk/(2*ndim+4)
      sbrgns = 0
      rgnvlt = zero
      rgnert = zero
      do 20 j=1,ndim
       center(j) = (a(j)+b(j))*half
       dif(j) = zero
       width(j) = (b(j)-a(j))*half
       dvcntl(j) = 1
       dvcntr(j) = 1
       oldcnt(j) = center(j)
       rgnvol = rgnvol*width(j)
   20 continue
c
c   end subroutine initialisation
c   basic rule initialisation
c
      rulcls = 2**ndim + 2*ndim*ndim + 2*ndim + 1
      funcls = rulcls
      if (maxpts.lt.rulcls) go to 560
      rlndim = ndim
      lamda2 = sqrt(9.d0/70.d0)
      lamda4 = sqrt(9.d0/10.d0)
      lamda5 = sqrt(9.d0/19.d0)
      weit1 = (12824.d0-9120.d0*rlndim+400.d0*rlndim*rlndim)/19683.d0
      weit2 = 980.d0/6561.d0
      weit3 = (1820.d0-400.d0*rlndim)/19683.d0
      weit4 = 200.d0/19683.d0
      weit5 = 6859.d0/19683.d0/twondm
      weitp1 = (729.d0-950.d0*rlndim+50.d0*rlndim**2)/729.d0
      weitp2 = 245.d0/486.d0
      weitp3 = (265.d0-100.d0*rlndim)/1458.d0
      weitp4 = 25.d0/729.d0
      ratio = (lamda2/lamda4)**2
c
c   end basic rule initialisation
      go to 100
c   divide subregion with largest error and prepare to use
c   basic rule on each portion
c
   40 subrgn = 1
      pointr = wrkstr(1)
      rgncls = rulcls
      rgnvol = twondm
      tpontr = pointr + 2
      do 60 j=1,ndim
       tpontr = tpontr + 2
       center(j) = wrkstr(tpontr-1)
       width(j) = wrkstr(tpontr)
       dvcntr(j) = 1
       dvcntl(j) = 1
       oldcnt(j) = center(j)
       rgnvol = rgnvol*width(j)
   60 continue
      dvaxes = wrkstr(pointr+2)
      if (dvaxes.lt.0) go to 600
   80 dvaxis = dvaxes
      dvaxes = dvaxis/(ndim+1)
      dvaxis = dvaxis - (ndim+1)*dvaxes
      dvcntl(dvaxis) = 2*dvcntl(dvaxis)
      rgncls = rgncls*2
      if (dvaxes.gt.0) go to 80
      if (funcls+rgncls.gt.maxpts) go to 580
      if (rgncls/rulcls+sbrgns-1.gt.mxrgns) dvflag = 2
      funcls = funcls + rgncls
c      print *,funcls
      abserr = abserr - wrkstr(pointr)
      finval = finval - wrkstr(pointr+1)
c
c   begin basic rule
  100 do 120 j=1,ndim
       z(j) = center(j)
  120 continue
      sum1 = functn(ndim,z)
      sum2 = zero
      sum3 = zero
      do 140 j=1,ndim
       z(j) = center(j) - lamda2*width(j)
       f1 = functn(ndim,z)
       z(j) = center(j) + lamda2*width(j)
       f2 = functn(ndim,z)
       z(j) = center(j) - lamda4*width(j)
       f3 = functn(ndim,z)
       z(j) = center(j) + lamda4*width(j)
       f4 = functn(ndim,z)
       sum2 = sum2 + f1 + f2
       sum3 = sum3 + f3 + f4
       df1 = f1 + f2 - two*sum1
       df2 = f3 + f4 - two*sum1
       dif(j) = dif(j) + abs(df1-ratio*df2)
       z(j) = center(j)
  140 continue
      sum4 = zero
      do 200 j=2,ndim
       z(j-1) = center(j-1) - lamda4*width(j-1)
       do 160 k=j,ndim
	  z(k) = center(k) - lamda4*width(k)
	  sum4 = sum4 + functn(ndim,z)
	  z(k) = center(k) + lamda4*width(k)
	  sum4 = sum4 + functn(ndim,z)
	  z(k) = center(k)
  160  continue
       z(j-1) = center(j-1) + lamda4*width(j-1)
       do 180 k=j,ndim
	  z(k) = center(k) - lamda4*width(k)
	  sum4 = sum4 + functn(ndim,z)
	  z(k) = center(k) + lamda4*width(k)
	  sum4 = sum4 + functn(ndim,z)
	  z(k) = center(k)
  180  continue
       z(j-1) = center(j-1)
  200 continue
      sum5 = zero
      do 220 j=1,ndim
       z(j) = center(j) - lamda5*width(j)
  220 continue
  240 do 260 j=2,ndim
       if (z(j-1).lt.center(j-1)+width(j-1)) go to 280
       z(j-1) = center(j-1) - lamda5*width(j-1)
       z(j) = z(j) + two*lamda5*width(j)
  260 continue
      if (z(ndim).gt.center(ndim)+width(ndim)) go to 300
  280 sum5 = sum5 + functn(ndim,z)
      z(1) = z(1) + two*lamda5*width(1)
      go to 240
  300 rgnval = rgnvol*(weit1*sum1+weit2*sum2+weit3*sum3+weit4*
     * sum4+weit5*sum5)
      rgncmp = rgnvol*(weitp1*sum1+weitp2*sum2+weitp3*sum3+weitp4*
     * sum4)
      rgnerr = abs(rgnval-rgncmp)
c
c   end basic rule
c   store results of basic rule application
c
      rgnvlt = rgnvlt + rgnval
      rgnert = rgnert + rgnerr
      finval = finval + rgnval
      abserr = abserr + rgnerr
      if (dvflag.eq.0) go to 340
      if (dvflag.eq.2) go to 500
      pointr = mxrgns + sbrgns*(2*ndim+3) + 1
      sbrgns = sbrgns + 1
      wrkstr(sbrgns) = pointr
      subrgn = sbrgns
      tpontr = pointr + 2
      do 320 j=1,ndim
       tpontr = tpontr + 2
       wrkstr(tpontr-1) = center(j)
       wrkstr(tpontr) = width(j)
  320 continue
  340 wrkstr(pointr) = rgnert
      wrkstr(pointr+1) = rgnvlt
c   determine axis along which fourth difference is largest
      difmax = zero
      do 380 j=1,ndim
       if (difmax.gt.dif(j)) go to 360
       difmax = dif(j)
       dvaxis = j
  360	    dif(j) = zero
  380 continue
      tpontr = pointr + 2*(dvaxis+1)
      wrkstr(tpontr) = width(dvaxis)*half
      wrkstr(tpontr-1) = center(dvaxis) - wrkstr(tpontr)
      if (dvflag.ne.2) go to 400
      dvaxes = wrkstr(pointr+2)
      if (dvaxes.gt.maxaxs) dvaxes = -1
      dvaxis = dvaxis + (ndim+1)*dvaxes
  400 wrkstr(pointr+2) = dvaxis
      if (dvflag.eq.1) go to 460
c   determine the position in the parially ordered list of
c   the subregion which replaces most recently divided subregion
  420 subtmp = 2*subrgn
      if (subtmp.gt.sbrgns) go to 480
      tpontr = wrkstr(subtmp)
      if (subtmp.eq.sbrgns) go to 440
      tpontp = wrkstr(subtmp+1)
      if (wrkstr(tpontr).ge.wrkstr(tpontp)) go to 440
      subtmp = subtmp + 1
      tpontr = tpontp
  440 if (rgnert.ge.wrkstr(tpontr)) go to 480
      wrkstr(subtmp) = pointr
      wrkstr(subrgn) = tpontr
      subrgn = subtmp
      go to 420
c   when working storage is not used up, determine the
c   position in the partially ordered list for the description
c   of other portion(s) of most recently divided subregion
  460 subtmp = subrgn/2
      if (subtmp.lt.1) go to 480
      tpontr = wrkstr(subtmp)
      if (rgnert.le.wrkstr(tpontr)) go to 480
      wrkstr(subtmp) = pointr
      wrkstr(subrgn) = tpontr
      subrgn = subtmp
      go to 460
  480 rgnvlt = zero
      rgnert = zero
      if (dvflag.eq.2) go to 540
      dvflag = 1 - dvflag
c   count to determine the next part of the recently divided
c   subregion for application of the basic rule
  500 center(1) = center(1) + two*width(1)
      dvcntr(1) = dvcntr(1) + 1
      do 520 j=2,ndim
       if (dvcntr(j-1).le.dvcntl(j-1)) go to 100
       dvcntr(j-1) = 1
       center(j-1) = oldcnt(j-1)
       dvcntr(j) = dvcntr(j) + 1
       center(j) = center(j) + two*width(j)
  520 continue
      if (dvcntr(ndim).le.dvcntl(ndim)) go to 100
      center(ndim) = oldcnt(ndim)
      if (dvflag.eq.2) go to 340
c
c   end ordering of basic rule results
c   make checks for possible termination of routine
c
  540 acc = abserr/abs(finval)
      if (acc.gt.eps .or. funcls.lt.minpts) go to 40
c
c   loop back to apply basic rule
c
c   termination point, set ifail and return
c
      ierror = 0
      go to 620
  560 ierror = 1
      go to 620
  580 ierror = 2
      go to 620
  600 ierror = 3
  620 minpts = funcls
      ifail = p01aae(ifail,ierror,srname)
      return
      end


*
*...cern library routine e104 (interpolation) :
*
      function dfint(narg,arg,nent,ent,table)
      implicit double precision (a-h,o-z)
      dimension arg(narg),nent(narg),ent(1000),table(100500)
      dimension d(narg),ncomb(narg),ient(narg)
      kd=1
      m=1
      ja=1
	 do 5 i=1,narg
      ncomb(i)=1
      jb=ja-1+nent(i)
	 do 2 j=ja,jb
      if (arg(i).le.ent(j)) go to 3
    2 continue
      j=jb
    3 if (j.ne.ja) go to 4
      j=j+1
    4 jr=j-1
      d(i)=(ent(j)-arg(i))/(ent(j)-ent(jr))
      ient(i)=j-ja
      kd=kd+ient(i)*m
      m=m*nent(i)
    5 ja=jb+1
      dfint=0.d0
   10 fac=1.d0
      iadr=kd
      ifadr=1
	 do 15 i=1,narg
      if (ncomb(i).eq.0) go to 12
      fac=fac*(1.d0-d(i))
      go to 15
   12 fac=fac*d(i)
      iadr=iadr-ifadr
   15 ifadr=ifadr*nent(i)
      dfint=dfint+fac*table(iadr)
      il=narg
   40 if (ncomb(il).eq.0) go to 80
      ncomb(il)=0
      if (il.eq.narg) go to 10
      il=il+1
	 do 50	k=il,narg
   50 ncomb(k)=1
      go to 10
   80 il=il-1
      if(il.ne.0) go to 40
      return
      end


CDECK  ID>, FSPEN.
****************** fspen **************************************

      double precision function fspen(x)
      implicit double precision(a-h,o-z)
      data f1/1.644934d0/
      if(x)8,1,1
  1   if(x-.5d0)2,2,3
    2 fspen=fspens(x)
      return
    3 if(x-1.d0)4,4,5
    4 fspen=f1-log(x)*log(1.d0-x+1d-10)-fspens(1.d0-x)
      return
    5 if(x-2.d0)6,6,7
    6 fspen=f1-0.5d0*log(x)*log((x-1.d0)**2/x)+fspens(1.d0-1.d0/x)
      return
    7 fspen=2.d0*f1-0.5d0*log(x)**2-fspens(1.d0/x)
      return
    8 if(x+1.d0)10,9,9
   9  fspen=-0.5d0*log(1.d0-x)**2-fspens(x/(x-1.d0))
      return
  10  fspen=-0.5d0*log(1.-x)*log(x**2/(1.d0-x))-f1+fspens(1.d0/(1.d0-x))
      return
      end

CDECK  ID>, FSPENS.
****************** fspens *************************************

      double precision function fspens(x)
c
c    spence function
c
      implicit double precision(a-h,o-z)
      f=0.d0
      a=1.d0
      an=0.d0
      tch=1.d-16
  1   an=an+1.d0
      a=a*x
      b=a/an**2
      f=f+b
      if(b-tch)2,2,1
  2   fspens=f
      return
      end


      double precision function x02aae(x)
      implicit double precision(a-h,o-z)
c     nag copyright 1975
c     mark 4.5 release
c+self,if=ibm.
cc     for ibm/360/370/3090
c      data z/z3380000000000000/
c      x02aae = z
c     for sun
      data z/1.1d-16/
      x02aae = z
c     * eps *
c     returns the value eps where eps is the smallest
c     positive
c     number such that 1.0 + eps > 1.0
c     the x parameter is not used
c     for icl 1900
c     x02aae = 2.0**(-37.0)
c+self,if=pc.
c     for pdp11
c      x02aae=2.d0**(-23.d0)
c+self.

      return
      end
c
      integer  function x02bbe(x)
      implicit double precision(a-h,o-z)
c     nag copyright 1975
c     mark 4.5 release
*     real x
c     * maxint *
c     returns the largest integer representable on the computer
c     the x parameter is not used
c     for icl 1900
c      x02bbe = 8388607
c     for ibm,sun,vax,ibm pc/386/486
       x02bbe = 2147483647
c   for pdp11
c     x02bbe=32767
      return
      end

      integer function p01aae(ifail, error, srname)
c     mark 1 release.  nag copyright 1971
c     mark 3 revised
c     mark 4a revised, ier-45
c     mark 4.5 revised
c     mark 7 revised (dec 1978)
c     returns the value of error or terminates the program.
      integer error, ifail, nout
      character*8 srname
c     test if no error detected
      if (error.eq.0) go to 20
c     determine output unit for message
      call x04aae (0,nout)
c     test for soft failure
      if (mod(ifail,10).eq.1) go to 10
c     hard failure
      write (nout,99999) srname, error
c     stopping mechanism may also differ
      stop
c     soft fail
c     test if error messages suppressed
   10 if (mod(ifail/10,10).eq.0) go to 20
      write (nout,99999) srname, error
   20 p01aae = error
      return
99999 format (1h0, 38herror detected by nag library routine , a8,
     * 11h - ifail = , i5//)
      end
      subroutine x04aae(i,nerr)
c     mark 7 release. nag copyright 1978
c     mark 7c revised ier-190 (may 1979)
c     if i = 0, sets nerr to current error message unit number
c     (stored in nerr1).
c     if i = 1, changes current error message unit number to
c     value specified by nerr.
c
c     *** note ***
c     this routine assumes that the value of nerr1 is saved
c     between calls.  in some implementations it may be
c     necessary to store nerr1 in a labelled common
c     block /ax04aa/ to achieve this.
c
c     .. scalar arguments ..
      integer i, nerr
c     ..
c     .. local scalars ..
      integer nerr1
c     ..
      data nerr1 /5/
      if (i.eq.0) nerr = nerr1
      if (i.eq.1) nerr1 = nerr
      return
      end
