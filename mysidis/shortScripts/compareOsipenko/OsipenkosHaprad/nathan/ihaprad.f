c HAPRAD v.2.0
c
c      version 2.0  01.04.1998
c
c
      subroutine ihaprad(bmom,ivecm,
     &iphi_radm,epsphirm,epstaum,epsrrm,
     &xmas,ymas,zmas,tmas,phimas,
     &sib,sig,delinf,delta,tai)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'epsil.inc'
      include 'epsmarch.inc'
      double precision bmom,epsphirm,epstaum,epsrrm,xmas,ymas,zmas,tmas,phimas,
     &sib,sig,delinf,delta,tai(3),tmom,snuc,
     &yma,ymi,amhh,sqnuq,p22max,tdmax,epspl
      integer ivecm,iphi_radm
      external sphih
      
      
      ipol=0       ! ipol - 1 -- long; 2 -- tran; 0 -- unpol <-- type of polarization
      iphi_had=0   ! iphi_had - integration over phi_{had} (1) or not (0)
      iphi_rad=iphi_radm
      ivec=ivecm
      model=3      ! model - model for FF and PDF: (1)-(3)
                   ! 1 - GRV and BKK for PDF and FF (refs [21] and [24] of our paper)
                   ! 2 - semi-exclusive case [9]
                   ! 3 - simple analytical models for PDF and FF
      ibs=10       ! ibs - model for pt slope parameter: (1,10)
                   ! 1  - exponential model: G from eq.(13)
		   ! 10 - power model: G from eq.(18)
      epsphir=epsphirm
      epstau=epstaum
      epsrr=epsrrm
      
      
      isf1=1
      isf2=isf20
      isf3=1
	    un=1.d0
	    pl=0.d0
	    pn=0.d0
	    qn=0.d0

      tmom=0.d0     ! tmom - momentum per nucleon
c      snuc=2.d0*(sqrt(tmom**2+amh**2)*sqrt(bmom**2+aml2)+bmom*tmom)
c      snuc=2.d0*(sqrt(tmom**2+amh**2)*sqrt(bmom**2)+bmom*tmom)
      snuc=2.d0*amp*bmom
c	print *,'tmas',tmas
      
	xs=xmas
	zdif=zmas
	tdif=tmas
	if(ymas.ge.0.d0)then
	  ys=ymas
	  y=snuc*xs*ys	 !  q2
	else
	  y=-ymas	 !  q2
	  ys=y/(snuc*xs)
	endif

      yma=1.d0/(1.d0+amp**2*xs/snuc)
      amhh=amhad(ivec)
      amc2=(amp+amhh)**2
      ymi=(amc2-amp**2)/(snuc*(1.d0-xs))
      if(ys.gt.yma.or.ys.lt.ymi.or.xs.gt.1.d0.or.xs.lt.0.d0)then
c       print *,' Warning! Wrong kinematics!!!! skeep the point!'
c       print *,' ys= ',ys
c       print *,' xs= ',xs
       return
      endif
      call conkin(snuc)
      ehad=anu*zdif
      sqnuq=sqrt(anu**2+y)
      
      if(ehad.lt.amhh)then
c      print *,' Warning! Wrong kinematics!!!! skeep the point!'
c      print *,' ehad =',ehad
      return
      endif
      
      pph=sqrt(ehad**2-amhh**2)
      
      if(tdif.ge.0.d0)then
      pth=tdif
      
      if(pph.lt.pth)then
c      print *,' Warning! Wrong kinematics!!!! skeep the point!'
c      print *,' pph =',pph
c      print *,' pth =',pth
      return
      endif
       
      plh=sqrt(pph**2-pth**2)
       
       if(pph.gt.pth) then
       an=an*sqly/2.d0/amp/plh
       else
       an=0.d0
       endif
       tdif=amhh**2-y+2.d0*(sqnuq*plh-anu*ehad)
       
c       print*,'pl: ',plh,tdif,(plh-(tdif+y-amhh**2+2.d0*anu*ehad)/2.d0/sqnuq)
       
      else
      plh=(tdif+y-amhh**2+2.d0*anu*ehad)/2.d0/sqnuq
      
      if(pph.lt.abs(plh))then
      epspl=sqrt((tdif*epsmarch/sqnuq)**2+
     &(2.d0*amhh**2*epsmarch/sqnuq)**2+
     &2.d0*(2.d0*anu*ehad*epsmarch/sqnuq)**2+
     &((tdif+y-amhh**2+2.d0*anu*ehad)/sqnuq*epsmarch)**2)/2.d0
      if(abs(pph-abs(plh)).gt.epspl)then
c      print *,' Warning! Wrong kinematics!!!! skeep the point!'
c	print *,' pph =',pph
c	print *,' plh =',plh,(pph-abs(plh))
c	if(abs(pph-plh).lt.1.d-9) stop
      return
      else
c      print*,'Zero pt!',plh,(pph-abs(plh)),epspl
      plh=sign(1.d0,plh)*pph
      endif
      endif
      
      pth=sqrt(pph**2-plh**2)
      
c      write(*,'(4f22.19,f7.4)') ehad,pph,pth,plh,anu
      
      endif

c      p22max=(sqrt(w2)-amp)**2
      p22max=w2-amhh**2
      p22=amp2+sx*(1.d0-zdif)+tdif
      if(p22.lt.amc2.or.p22.gt.p22max)then
c      print *,' Warning! Wrong kinematics!!!! skeep the point!'
c       print *,' p22 =',p22
c       print *,' amc2=',amc2
c       print *,' p22m=',p22max
       return
      endif

      tdmin=amhh**2-y+2.d0*( sqnuq*pph-anu*ehad)
      tdmax=amhh**2-y+2.d0*(-sqnuq*pph-anu*ehad)
      
c      print*,tdif,tdmin,tdmax,(tdif-tdmin)
      
c      stop
cc	tdif=tdif+tdmin

      if((tdif-tdmin).gt.epsmarch.or.tdif.lt.tdmax)then
c        print *,' Warning! Wrong kinematics!!!! skeep the point!'
c	print *,' tdif =',tdif
c	print *,' tdmax=',tdmax
c	print *,' tdmin=',tdmin,(tdif-tdmin)
	return
      endif
      
      
c      plh=(tdif+y-amhh**2+2.d0*anu*ehad)/2.d0/sqnuq
c      plh=(amhh**2-y+2.d0*(sqnuq*sqrt(pph**2-pth**2)-anu*ehad)+
c     &y-amhh**2+2.d0*anu*ehad)/(2.d0*sqnuq)
c      dum=sqrt(pph**2-pth**2)
c      plh=dum-(amp*dum+ehad-ehad)/amp
c      print*,'diff: ',plh,(plh-dum),sqnuq,tdif,anu,ehad
      
      
	
c      print *,y,xs,zdif,pth,pph,plh,tdif,anu,ehad,sqnuq
      
      call sphih(phimas,sib,sig,delinf,delta,tai)
      
      return
      end

      subroutine sphih(phih,sib,sig,delinf,delta,tai)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      double precision phih,sib,sibt,sig,delinf,delta,tai(3),costs,sints,phk1,
     &costx,sintx,phk2,tr,extai1
      integer il,infin,in

      phidif=phih

      costs=(s*(s-x)+2.d0*amp2*y)/sqls/sqly
      if((s*x*y-amp2*y**2-aml2*aly).gt.0.d0) then
      sints=2.d0*amp*sqrt(s*x*y-amp2*y**2-aml2*aly)/sqls/sqly
      else
      print*,'sphih: sints=NaN ',(s*x*y-amp2*y**2-aml2*aly)
      sints=0.d0
      endif
      vv10=(s*ehad-sqls*(costs*plh+sints*pth*cos(phidif)))/amp
      phk1=0.5d0*(s*ehad-sqls*costs*plh)/amp
      phk12=-0.5d0*sqls*sints*pth/amp
      costx=(x*(s-x)-2.d0*amp2*y)/sqlx/sqly
      if((s*x*y-amp2*y**2-aml2*aly).gt.0.d0) then
      sintx=2.d0*amp*sqrt(s*x*y-amp2*y**2-aml2*aly)/sqlx/sqly
      else
      print*,'sphih: sintx=NaN ',(s*x*y-amp2*y**2-aml2*aly)
      sintx=0.d0
      endif
      vv20=(x*ehad-sqlx*(costx*plh+sintx*pth*cos(phidif)))/amp
      phk2=0.5d0*(x*ehad-sqlx*costx*plh)/amp
      phkp=phk1+phk2
      phkm=phk2-phk1
c      print *,sints**2+costs**2
c      print *,sintx**2+costx**2

c
c delta is factorizing part of virtual and real leptonic bremsstrahlung
c
      call deltas(delta,delinf,tr)

      do 10 il=1,1
      infin=3
      if(ipol.eq.0)infin=1
      do 10 in=1,infin,2
     
      do 30 ita=1,2
c      write(9,'(10(1h*),'' ita = '',i2,10(1h*))')ita
c      write(*,'(10(1h*),'' ita = '',i2,10(1h*))')ita

      if(ita.eq.1)then
	 call bornin(sib)
	 call bornintest(sibt)
c	 print *,'sib1',sib
c	 print *,'sibt',sibt
c	 stop
      if(sib.eq.0.0) then
      tai(1)=0.d0
      goto 30
      endif
c	 stop
      endif

      call qqt(tai(ita))

  30  continue

      extai1=exp(alfa/pi*delinf)

      sig=sib*extai1*(1.d0+alfa/pi*(delta-delinf))
     & +tai(1)
     & +tai(2)
     
c	print  *,'dd ',((1.d0-zdif)*sx+tdif),sib,tai(2),(tai(2)/sib)

 10   continue

      return
      end

CDECK  ID>, CONKIN.
****************** conkin *************************************

      subroutine conkin(snuc)
c set of kinematical constants
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'pol.inc'
      double precision snuc,axy,sqn
      
      amt=amp
      aml2=aml**2
      al2=2.d0*aml2
      pi2=pi**2
      amh=amp
      ap=2.d0*amp
      amp2=amp**2
      ap2=2.d0*amp2
      
      s=snuc*amp/amh
      x=s*(1.d0-ys)
      sx=s-x
      sxp=s+x
      ym=y+al2
      tpl=s**2+x**2
      tmi=s**2-x**2
      w2=amp2+s-y-x
      als=s*s-al2*ap2
      alx=x*x-al2*ap2
      alm=y*y+4.d0*aml2*y
      aly=sx**2+4.d0*amp2*y
      if(als.lt.0.d0) print*,'conkin: als<0 ',als
      sqls=sqrt(max(0.d0,als))
      if(alx.lt.0.d0) print*,'conkin: alx<0 ',alx
      sqlx=sqrt(max(0.d0,alx))
      if(aly.lt.0.d0) print*,'conkin: aly<0 ',aly
      sqly=sqrt(max(0.d0,aly))
      if(alm.lt.0.d0) print*,'conkin: alm<0 ',alm
      sqlm=sqrt(max(0.d0,alm))
      allm=log((sqlm+y)/(sqlm-y))/sqlm
ccccccccccccccccccccccccccccccccccc
      anu=sx/ap
ccccccccccccccccccccccccccccccccccc
      axy=pi*(s-x)
c      an=2.*alfa**2/sqls*axy*barn*amh/amp
       an=pi*alfa**2*ys*sx*amp/2.d0/sqly*barn
c	print *,'an1',an
c      tamin=(sx-sqly)/ap2
      tamax=(sx+sqly)/ap2
      tamin=-y/amp2/tamax
      as=s/2.d0/aml/sqls
      bs=0.d0
      cs=-aml/sqls
      if(ipol.ne.2)then
      ae=amp/sqls
      be=0.d0
      ce=-s/ap/sqls
      else
      if((s*x*y-aly*aml2-amp2*y*y).gt.0.d0) then
      sqn=sqrt(s*x*y-aly*aml2-amp2*y*y)
      else
      print*,'conkin: sqn=NaN ',(s*x*y-aly*aml2-amp2*y*y)
      sqn=0.d0
      endif
      ae=(-s*x+ap2*ym)/sqls/sqn/2.d0
      be=sqls/sqn/2.d0
      ce=-(s*y+al2*sx)/sqls/sqn/2.d0
      endif
      apq=-y*(ae-be)+ce*sx
      apn=(y+4.d0*aml2)*(ae+be)+ce*sxp
      dk2ks=as*ym+al2*bs+cs*x
      dksp1=as*s+bs*x+cs*ap2
      dapks=2.d0*(al2*(as*ae+bs*be)+ap2*cs*ce+ym*(as*be+bs*ae)+
     &s*(as*ce+cs*ae)+x*(bs*ce+cs*be))
      return
      end

CDECK  ID>, BORNIN.
****************** bornin *************************************

      subroutine bornintest(sibort)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'phi.inc'
      double precision sibort,sfm0(8),q2,Eb,H1,H2,H3,H4,Eh,rt,rtz,gev2mb,semi
c      call strf(0.d0,0.d0,0.d0,sfm0)
      q2=y
      Eb=s/2.d0/amp
      call semi_inclusive_model(q2,xs,ys,zdif,pth**2,p22,plh,H1,H2,H3,H4)
      Eh=zdif*sx/2.d0/amp
      rt=1.d0-ys-amp*xs*ys/(2.d0*Eb)
      rtz=sqrt(rt/(1.d0+2.d0*amp*xs/(ys*Eb)))
      gev2mb=389.37929d0
c      semi=(1.0d+3)*gev2mb*(4.d0*pi*alfa**2*amp*Eb)/(q2**2)*
c     &Eh/plh*
      semi=(16.d0*an*amp*Eb)/(q2**2)*
     &Eh/ys/sx*
     &(xs*ys**2*H1+rt*H2+pth/sqrt(q2)*(2.d0-ys)*rtz*cos(phidif)*H3+
     &pth**2/q2*rtz**2*cos(2.d0*phidif)*H4)
      
c      print*,'born ',semi*1.d-3,(xs*ys**2*H1+rt*H2),
c     &(pth/sqrt(q2)*(2.d0-ys)*rtz*cos(phidif)*H3),
c     &(pth**2/q2*rtz**2*cos(2.d0*phidif)*H4),phidif,ys
c      print*,'btest ',Eb,Eh,plh,q2,xs,zdif,pth,phidif,H1,H2,H3,H4
c      stop
      
      sibort=semi
      return
      end

      subroutine bornin(sibor)
c
c     sibor is born cross section with polarized initial
c     lepton and polarized target
c     siamm is contribution of anomalous magnetic moment.
c
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'pol.inc'
      include 'print.inc'
      double precision sibor,sfm0(8),tm(8),ssum,aa,hi2
      integer isf
       ipri1=1
      call strf(0.d0,0.d0,0.d0,sfm0)
       ipri1=0

      hi2=0.25d0

	tm(1)=y
	tm(2)=(s*x-amp2*y)/2.d0
	tm(3)=(vv10*vv20-amhad(ivec)**2*y)/2.d0
	tm(4)=(s*vv20+x*vv10-zdif*sx*y)/2.d0
      aa=sx*(zdif-2.d0*amp*plh/sqly)/2.d0/amp2
      ssum=0.d0
      do 1 isf=isf1,isf2,isf3
	ssum=ssum+tm(isf)*sfm0(isf)
c	print *,isf
c	print *,ssum
    1 continue
c    	print *,'an2',an,ssum,y,tm(2),sfm0(2)
      sibor=ssum*an/y/y*2.d0
      return
      end

CDECK  ID>, DELTAS.
****************** deltas *************************************

      subroutine deltas(delta,delinf,tr)
c
c delta is factorizing part of virtual and real leptonic bremsstrahlung
c
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'phi.inc'
      double precision delta,delinf,tr,del1,del2,sum,aj0,deltai,ssh,xxh,alss,
     &alxx,sqlss,sqlxx,allss,allxx,dlm,delta0,delta_old,fspen,vacpol,
     &sfpr

      del1=-ym*(alm*allm**2/2.d0+2.d0*fspen(2.d0*sqlm/(y+sqlm))-pi2/2.d0)/sqlm
      del2=(3.d0*y/2.d0+4.d0*aml2)*allm-2.d0

      sum=vacpol(y)

      aj0=2.d0*(ym*allm-1.d0)
      deltai=aj0*log((p22-amc2)/aml/sqrt(p22))

      ssh=x+y-vv20
      xxh=s-y-vv10
      alss=ssh**2-2.d0*p22*al2
      alxx=xxh**2-2.d0*p22*al2
      if(alss.lt.0.d0) print*,'deltas: alss<0 ',alss
      sqlss=sqrt(max(0.d0,alss))
      if(alxx.lt.0.d0) print*,'deltas: alxx<0 ',alxx
      sqlxx=sqrt(max(0.d0,alxx))
      allss=log((sqlss+ssh)/(-sqlss+ssh))/sqlss
      allxx=log((sqlxx+xxh)/(-sqlxx+xxh))/sqlxx
      dlm=log(y/aml2)
      sfpr=dlm**2/2.d0-dlm*log(ssh*xxh/aml2/p22)-
     &(log(ssh/xxh))**2/2.d0+fspen(1.d0-p22*y/ssh/xxh)-pi2/3.d0
      delta0=(ssh*allss+xxh*allxx)/2.d0+sfpr
      delta_old=deltai+delta0+del1+del2+sum
      delinf=(dlm-1.d0)*log((p22-amc2)**2/ssh/xxh)
      tr=alfa/pi*(dlm-1.d0)

      delta=delinf+sum+(1.5d0*dlm-2.d0-0.5d0*log(xxh/ssh)**2+
     &		 fspen(1.d0-p22*y/ssh/xxh)-pi2/6.d0)

c      write(*,'(6hdelta:,2g12.4)')delta_old,delta

      return
      end

CDECK  ID>, VACPOL.
      double precision function vacpol(t)
      implicit none
      include 'haprad_consts.inc'
      double precision t,am2(3),suml,a2,sqlmi,allmi,aaa,bbb,ccc,sumh
      integer i
c
c    am2 : squared masses of charge leptons
c
      data am2/0.26110d-6,0.111637d-1,3.18301d0/

      suml=0.d0
      do 10 i=1,3
	 a2=2.d0*am2(i)
	 sqlmi=sqrt(t*t+2.d0*a2*t)
	 allmi=log((sqlmi+t)/(sqlmi-t))/sqlmi
  10  suml=suml+2.d0*(t+a2)*allmi/3.d0-10.d0/9.d0+4.d0*a2*(1.d0-a2*allmi)/3.d0/t
      if(t.lt.1.d0)then
	aaa = -1.345d-9
	bbb = -2.302d-3
	ccc =  4.091d0
      elseif(t.lt.64.d0)then
	aaa = -1.512d-3
	bbb = -2.822d-3
	ccc =  1.218d0
      else
	aaa = -1.1344d-3
	bbb = -3.0680d-3
	ccc =  9.9992d-1
      endif
      sumh = -(aaa+bbb*log(1.d0+ccc*t))*2.d0*pi/alfa

      vacpol=suml+sumh

      return
      end


CDECK  ID>, QQT.
****************** qqt ****************************************

      subroutine qqt(tai)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'epsil.inc'
      double precision tai,qqtphi,rv2tr,
     &phiar(4),tar(6),ta1,ta2,ot,otr,am(2),bm(2),wrk(500),rere,re
      integer iph,ico,id,mir,ma
      external qqtphi
      external rv2tr
	
      if(ita.eq.1)then
      if(iphi_rad.eq.1)then
	call simpsx(0.d0,(2.d0*pi),150,epsphir,qqtphi,tai)
	tai=an*alfa*tai/pi**2/4.d0/sqly
      elseif(iphi_rad.eq.0)then
	tai=an*alfa/pi*qqtphi(0.d0)/2.d0/sqly
      endif
c	print *,'tai(1)=',tai
c	stop
      else
      ta1=-y/s	
      ta2=y/x	
      phiar(1)=0.d0
      phiar(2)=0.01d0*pi
      phiar(3)=2.d0*pi-0.01d0*pi
      phiar(4)=2.d0*pi
      tar(1)=tamin
      tar(2)=ta1-0.15d0*(ta1-tamin)
      tar(3)=ta1+0.15d0*(ta2-ta1)
      tar(4)=ta2-0.15d0*(ta2-ta1)
      tar(5)=ta2+0.15d0*(tamax-ta2)
      tar(6)=tamax
      ot=1.d-3

      rere=0.d0
      do iph=1,3
      do ico=1,5

      am(1)=tar(ico)
      bm(1)=tar(ico+1)
      am(2)=phiar(iph)
      bm(2)=phiar(iph+1)
       if(am(2).gt.bm(2))write(*,*)' am(2)<bm(2)'
       if(am(1).gt.bm(1))write(*,*)' am(1)<bm(2)'

      id=1
      mir=10000
      ma=10*mir
c      print*,'start d01fce integration!'
      call d01fce(2,am,bm,mir,ma,rv2tr,ot,otr,500,wrk,re,id)
c      write(9,'(1x,''tai:'',2i3,g13.4,f8.4,i9,i3)')ico,iph,re,otr,mir,id
c      write(*,'(1x,''tai:'',2i3,g15.6,f8.4,i9,i3)')ico,iph,re,otr,mir,id
      rere=rere+re
      enddo
      enddo
	tai=-alfa/(64.d0*pi**5*sqly*amp)*an*rere
      endif
      return
      end

****************** rv2tr **************************************

      double precision function rv2tr(ndim,arg)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'phi.inc'
      include 'ex.inc'
      double precision arg(15),tm(8,6),sfm(4),
     &vv,fwiw,ta,dmu,r,tldq2,tldw2,tldtm,podinlz,pres
      integer ndim,isf,irr
      if(ndim.lt.2 .or. ndim.gt.15) stop
      phirad=arg(2)
      ta=arg(1)
      amh2=amhad(ivec)**2
      amu2=amp2
      vv=(1.d0-zdif)*sx+tdif
      dmu=(ehad-plh*(sx-ta*ap2)/sqly
     &-ap2*pth*sqrt((ta-tamin)*(tamax-ta))*cos(phidif-phirad)/sqly)/amp	
	fwiw=1.d0+ta-dmu
      r=vv/fwiw
c      tt=-y+amhad(ivec)**2-vv1+vv2
c	print *,'t',tt
c	print *,'t',tdif
      tldq2=y+r*ta
      tldw2=w2-r*(1.d0+ta)
      tldtm=tdif-r*(ta-dmu)
       call sffun(sfm,tldq2,tldw2,tldtm)
       call tails(ta,tm,dmu)
      podinlz=0.d0
      do  isf=1,4
      do  irr=1,3
	 pres=sfm(isf)*tm(isf,irr)/tldq2**2*r**(irr-2)
	 podinlz=podinlz+pres
      enddo
      enddo
      rv2tr=podinlz/fwiw
      return
      end


      subroutine sffun(sfm,q2,w2,t)
      implicit none
      include 'haprad_consts.inc'
      include 'ex.inc'
      double precision sfm(4),q2,w2,t,sqw2,sx,aly,sxt,tq,sqlw,sqll,cspion,
     &st,sl,stt,slt,sltp,sfm10,sfm20,sfm2tl,sfm4tl,sfm4tt,
     &sfm3tt,sfm2tt,sfm5tl,coetr
      sqw2=sqrt(w2)
      sx=w2+q2-amp2
      aly=sx**2+4.d0*amp2*q2
      sxt=sx+t+amp2-amu2
      tq=t+q2-amh2

      if(((W2-amu2-amh2)**2-4.d0*amu2*amh2).lt.0.d0) print*,'sffun: sqlw=NaN ',
     &((W2-amu2-amh2)**2-4.d0*amu2*amh2)
      sqlw=sqrt(max(0.d0,((W2-amu2-amh2)**2-4.d0*amu2*amh2)))
      if((q2*sxt**2-sxt*sx*tq-amp2*tq**2-amh2*aly).lt.0.d0) print*,'ssffun: qll=NaN ',
     &(q2*sxt**2-sxt*sx*tq-amp2*tq**2-amh2*aly)
      sqll=sqrt(max(0.d0,(q2*sxt**2-sxt*sx*tq-amp2*tq**2-amh2*aly)))
      cspion=(2.d0*tq*w2+(sx-2.d0*q2)*(w2+amh2-amu2))/sqlw/sqrt(aly)

c      Eh=sxt/2./amp
c      anu=sx/2./amp
c      qm=sqrt(anu**2+q2)
c      Eh_cm=(w2+amh2-amp2)/(2.d0*sqw2)
c      ph_cm=sqrt(Eh_cm**2-amh2)
c      bet=qm/(anu+amp)
c      gam=(anu+amp)/sqw2
c      c_th_cm=(Eh-gam*Eh_cm)/(bet*gam*ph_cm)
c        print*,'costhcm= ',cspion,c_th_cm
c	stop

c Exclusive peak model (cross sections sigma_L,T,LT... from MAID2003)
      call exclusive_model(q2,sqw2,cspion,
     &st,sl,stt,slt,sltp)
      
c Structure functions
      if(aly.gt.0.0.and.sqll.gt.0.0.and.sqlw.gt.0.0) then
      sfm10=(st-stt)
      sfm20=4.d0*(st+sl)*q2/aly
      sfm2tl=2.d0*slt*sqrt(q2)*(-sx*tq+2*q2*sxt)/(aly*sqll)
      sfm4tl=-slt*sqrt(q2)/sqll
      sfm4tt=-2.d0*stt*(-sx*tq+2*q2*sxt)/sqll**2
      sfm3tt=2.d0*stt*aly/sqll**2
      sfm2tt=2.d0*stt*((-sx*tq+2*q2*sxt)**2-2*q2*sqll**2)/(aly*sqll**2)
      sfm5tl=-sltp*sqrt(q2)/sqll


       coetr=16.d0*pi*(w2-amp2)*w2/(alfa*sqlw)/barn*1.d3
       sfm(1)=coetr*sfm10
       sfm(2)=coetr*(sfm20+sfm2tl+sfm2tt)
       sfm(3)=coetr*sfm3tt
       sfm(4)=coetr*(sfm4tl+sfm4tt)
       else
       sfm(1)=0.d0
       sfm(2)=0.d0
       sfm(3)=0.d0
       sfm(4)=0.d0
       endif
       
       if(sfm(2).ne.sfm(2)) print*,'sffun: ',coetr,st,sl,stt,slt,sltp,
     &q2,aly,sx,sqw2,cspion,sxt,sqll
       
       
      return
      end

      double precision function qqtphi(phi)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'epsil.inc'
      double precision phi,ta1,ta2,tar(6),res,re,ep
      integer i
      external rv2ln
	phirad=phi
      ep=1.d-12
      ta1=-y/s  
      ta2=y/x  
      tar(1)=tamin
      tar(2)=ta1-0.15d0*(ta1-tamin)
      tar(3)=ta1+0.15d0*(ta2-ta1)
      tar(4)=ta2-0.15d0*(ta2-ta1)
      tar(5)=ta2+0.15d0*(tamax-ta2)
      tar(6)=tamax
	res=0.d0
       do i=1,5
      call simptx(log(xs+tar(i))+ep,log(xs+tar(i+1))-ep,100,epstau,rv2ln,re)
      res=res+re
       enddo 
	qqtphi=res
      return
      end

CDECK  ID>, TAILS.
****************** tails **************************************

      subroutine tails(ta,tm,amu)
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'pol.inc'
      double precision ta,tm(8,6),b2,b1,c1,c2,bb,sc1,sc2,bi12,bi1pi2,
     &bis,bir,b1i,b11i,sqrtmb,z1,z2,hi2,amhh,amh2,zh,
     &vvp,vvm,amu
      if(iphi_rad.eq.0.and.ita.eq.1)then
       b2=(-aly*ta+sxp*sx*ta+2.d0*sxp*y)/2.d0
       b1=(-aly*ta-sxp*sx*ta-2.d0*sxp*y)/2.d0
       c1=-(4.d0*(amp2*ta**2-sx*ta-y)*aml2-(s*ta+y)**2)
       c2=-(4.d0*(amp2*ta**2-sx*ta-y)*aml2-(ta*x-y)**2)
       bb=1.d0!/sqly
       if(c1.lt.0.d0) print*,'tails: sc1=NaN ',c1
       sc1=sqrt(max(0.d0,c1))
       if(c2.lt.0.d0) print*,'tails: sc2=NaN ',c2
       sc2=sqrt(max(0.d0,c2))
       bi12=sqly*(sxp*(sx*ta+2.d0*y))/(sc1*sc2*(sc1+sc2))
       bi1pi2=sqly/sc2+sqly/sc1
       bis=sqly*(-b1/sc1/c1+b2/sc2/c2)*aml2
       bir=sqly*(b2/sc2/c2+b1/sc1/c1)*aml2
       b1i=-sqly*b1/aly/sqly
       b11i=sqly*(3.d0*b1**2-aly*c1)/2.d0/aly**2/sqly
      else
       if(((ta-tamin)*(tamax-ta)*(s*x*y-y**2*amp2-aml2*aly)).gt.0.d0)then
       sqrtmb=sqrt((ta-tamin)*(tamax-ta)*(s*x*y-y**2*amp2-aml2*aly))
       else
       print*,'tails: sqrtmb=NaN ',((ta-tamin)*(tamax-ta)*(s*x*y-y**2*amp2-aml2*aly))
       sqrtmb=0.d0
       endif
       z1=(y*sxp+ta*(s*sx+ap2*y)-ap*cos(phirad)*sqrtmb)/aly
       z2=(y*sxp+ta*(x*sx-ap2*y)-ap*cos(phirad)*sqrtmb)/aly
       bb=1.d0!/sqly/pi
       bi12=bb/(z1*z2)
       bi1pi2=bb/z2+bb/z1
       bis=(bb/z2**2+bb/z1**2)*aml2
       bir=(bb/z2**2-bb/z1**2)*aml2
       b1i=bb*z1
       b11i=bb*z1**2
      endif
      hi2=bis-ym*bi12

      amhh=amhad(ivec)
      amh2=amhh**2

      zh=zdif
	vvp=(vv10+vv20)/2.d0
	vvm=(vv10-vv20)/2.d0
	tm(1,1)=4.d0*y*hi2
	tm(1,2)=4.d0*ta*hi2
	tm(1,3)=-2.d0*(bi12*ta**2+2.d0*bb)
	tm(2,1)=2.d0*hi2*(s*x-amp2*y)
	tm(2,2)=(bi1pi2*sx*sxp-bi12*ta*sxp**2+2.d0*bir*sxp+
     &2.d0*hi2*(sx-2*amp2*ta))/2.d0
	tm(2,3)=(bi12*ta*(2.d0*amp2*ta-sx)-bi1pi2*sxp+4.d0*amp2*bb)/2.d0
	tm(3,1)=2.d0*hi2*(vv10*vv20-amh2*y)
	tm(3,2)=-2.d0*((amh2*ta-amu*vvm)*hi2-bir*amu*vvp-
     &bi1pi2*vvm*vvp+bi12*ta*vvp**2)
	tm(3,3)=bi12*ta*(amh2*ta-amu*vvm)-bi1pi2*amu*vvp+
     &2.d0*amh2*bb
	tm(4,1)=-2.d0*(vv10*sx-2.d0*s*vvp+y*sx*zh)*hi2
	tm(4,2)=-2.d0*bi12*ta*vvp*sxp+bi1pi2*(sxp*vvm+sx*vvp)+
     &bir*(amu*sxp+2.d0*vvp)+hi2*(sx*(amu-2.d0*ta*zh)+2.d0*vvm)
	tm(4,3)=bi12*ta*(sx*(ta*zh-amu/2d0)-vvm)-bi1pi2*(amu/2d0*sxp+vvp)+
     &2.d0*sx*zh*bb
	
      return
      end




CDECK  ID>, RV2LN.
****************** rv2ln **************************************

      double precision function rv2ln(taln)
c
c     integrand (over ta )
c
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'epsil.inc'
      include 'amf2.inc'
      double precision taln,podinl,ta,costk,sintk,d2kvir,phka,phkb,
     &factor,rmin,rmax,res,rv,vyv(0:1000),DSIMPS,aval,bval
      integer i
      logical nonzero
      external podinl
      
      ta=exp(taln)-y/sx
      taa=ta
ccccccccccccccccccccccccccccccccccccccccccc
      costk=(sx-ap2*ta)/sqly
      if(abs(costk).le.1.d0) then
      sintk=sqrt(1.d0-costk**2)
      else
      print*,'rv2ln: costk>1 ',costk
      sintk=0.d0
      endif
      d2kvir=(ehad-plh*costk-pth*sintk*cos(phirad-phidif))/amp
      phka=0.5d0*(ehad-plh*costk)/amp
      phkb=0.5d0*(-pth*sintk)/amp
      daa=d2kvir
      factor=1.d0+ta-d2kvir
ccccccccccccccccccccccccccccccccccccccccccc

       call tails(ta,tm,d2kvir)

      if(ita.eq.1)then
	 call strf(0.d0,0.d0,0.d0,sfm0)
	 rmin=1.d-8
	 rmax=(p22-amc2)/factor
	 
cc Check minimum r
cc      print*,'rmin1: ',rmin,rmax
c      nonzero=.false.
c      do i=1,4
c      if(sfm0(i).ne.0.d0) nonzero=.true.
c      enddo
c      if(nonzero) then
c      if((ap*d2kvir-2.d0*ehad).gt.0.d0) then
c      aval=(2.d0*ehad*sx-ap*(amhad(ivec)**2-y-tdif))/
c     &(ap*d2kvir-2.d0*ehad)
c      bval=pph**2*2.d0*sqrt(sx**2+(ap*y)**2)/
c     &(ap*d2kvir-2.d0*ehad)
cc      rmin=aval-bval
c      rmin=(abs(aval)+abs(bval))*1.d-7
c      else
c      aval=(2.d0*ehad*sx-ap*(amhad(ivec)**2-y-tdif))
c      bval=pph**2*2.d0*sqrt(sx**2+(ap*y)**2)
c      rmin=(abs(aval)+abs(bval))*1.d-7
c      endif
c      endif
cc      print*,'rmin2: ',rmin,rmax
	 
	 
c	 call qvnc8(podinl,rmin,rmax,1.d-4,1.d-9,res,er,nn,fl,3500)
c	 call dqn32(rmin,rmax,podinl,res)
      call simpux(rmin,rmax,100,epsrr,podinl,res)
      
cc      print*,'start integration '
c      do i=0,100
c      rv=rmin+dble(i)*(rmax-rmin)/1.d2
cc      print*,'start integration: ',i
c      vyv(i)=podinl(rv)
cc      print*,rv,vyv(i)
c      enddo
c      res=DSIMPS(vyv,rmin,rmax,100)
cc      print*,'-----------------------------',res
	 
c	 print*,'###################INTEGRATE !'
	 
      elseif(ita.eq.2)then
	 res=podinl((p22-amp2)/factor)/factor
c     else
c	 aa=amt/amp
c	 if(ita.eq.3)aa=1.
c	 res=podinl((sx-y/aa)/(1.d0+ta/aa))/(1.+ta/aa) /aa**2
      endif
      rv2ln=res*(y/sx+ta)
      
c      print*,rv2ln,res,rmin,rmax,epsrr

      return
      end


CDECK  ID>, PODINL.
****************** podinl *************************************

      double precision function podinl(r)
c
c     integrand (over r )
c
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'amf2.inc'
      double precision r,pp,pres,sfm(8),ta,d2kvir,rm
      integer isf,irr,i
      logical reget
      ta=taa
      d2kvir=daa
      call strf(ta,d2kvir,r,sfm)
      podinl=0.d0
      
c Check lower bound
      if(ita.eq.1) then
      rm=r
1010  reget=.false.
      do i=1,4
      if(sfm(i).eq.0.d0.and.sfm0(i).ne.0.d0.and.r.lt.1.d-6) reget=.true.
      if(sfm(i).eq.0.d0.and.sfm0(i).ne.0.d0.and.r.lt.1.d-6) print*,i,sfm(i),sfm0(i)
      enddo
      if(reget) then
      rm=rm+1.d-8
c      print*,'regetting ',rm,r
      call strf(ta,d2kvir,rm,sfm)
      stop
      goto 1010
      endif
      endif
      
c      print*,'podinl start ',isf1,isf2,isf3,i1(isf)+i2(isf)-1
      
      do 11 isf=isf1,isf2,isf3
      do 1 irr=1,(i1(isf)+i2(isf)-1)
      pp=sfm(isf)
      if(irr.eq.1.and.ita.eq.1) pp=pp-sfm0(isf)*(1.d0+r*ta/y)**2
      pres=pp*r**(irr-2)/(y+r*ta)**2
      podinl=podinl-tm(isf,irr)*pres
      
c      if(irr.eq.1) print*,'pod: ',isf,irr,sfm0(isf),sfm(isf),pp,pres,r**(irr-2)

    1 continue
   11 continue

c      print*,'podinl: ',r,podinl

      return
      end

CDECK  ID>, STRF.
      subroutine strf(ta,d2kvir,rr,sfm)
c
c     the programm calculates deep inelastic (ita=1),
c     elastic (ita=2), quasielastic (ita=3) structure functions
c     in kinematical point (ta,rr).
c	   rr=sx-tt,
c	   ta=(t-y)/rr,
c    where tt=t+amf2-amp2, amf2 is invarint mass of final hadrons
c
      implicit none
      include 'haprad_consts.inc'
      include 'sxy.inc'
      include 'tail.inc'
      include 'phi.inc'
      include 'print.inc'
      include 'epsmarch.inc'
      double precision ta,d2kvir,rr,sfm(8),tldq2,tldtd,tldaly,tldsqly,
     &phq,tldnu,aks,zh,tldsq,tldplh,b1,b2,b3,b4,epspt2,
     &epsnu,epst,epsphq,epsq,epspl,
     &tldpt2,H1z,H2z,H3z,H4z,epsi,aa,h1,h2,h3,h4,amhh,dum,tldp22
      double precision a,tlde,tldy
      integer i


     
      do i=1,8
      sfm(i)=0.d0
      enddo

      tldq2=y+rr*ta
      tldtd=tdif-rr*(ta-d2kvir)
      tldaly=(sx-rr)**2+4.d0*amp2*tldq2
      tldsqly=sqrt(tldaly)
      tldp22=p22-rr*(1.d0+ta-d2kvir)
      amhh=amhad(ivec)
      phq=(amhh**2-tldq2-tldtd)/2.d0


c      print*,'---------------Begin-----------------'
c      write(*,'(3f22.19)') sx,rr,ap
      
      tldnu=(sx-rr)/ap
      aks=tldq2/ap/tldnu
      zh=ehad/tldnu
      tldsq=sqrt(tldq2+tldnu**2)
c      tldplh=(tldtd+tldq2-amhad(ivec)**2+2.d0*tldnu*ehad)/2.d0/tldsq
      tldplh=(ehad*tldnu-phq)/tldsq
      tldpt2=pph**2-tldplh**2
      
      
      epsnu=sqrt((sx*epsmarch/ap)**2+(rr*epsmarch/ap)**2+
     &((sx-rr)/ap*epsmarch)**2)
      epst=sqrt((tdif*epsmarch)**2+((ta-d2kvir)*rr*epsmarch)**2+
     &(rr*ta*epsmarch)**2+(rr*d2kvir*epsmarch)**2)
      epsphq=sqrt((2.d0*amhh**2*epsmarch)**2+(tldq2*epsmarch)**2+epst**2)/2.d0
      epsq=1.d0/2.d0/sqrt(tldq2+tldnu**2)*sqrt((tldq2*epsmarch)**2+
     &(2.d0*tldnu**2*epsnu)**2)
      epspl=sqrt((ehad*epsmarch*tldnu/tldsq)**2+(ehad*epsnu/tldsq)**2+
     &(epsphq/tldsq)**2+((ehad*tldnu-phq)/(tldsq**2)*epsq)**2)
      epspt2=2.d0*sqrt(pph**4*epsmarch**2+tldplh**4*epspl**2)
c      print*,'pt2= ',tldpt2,epspt2,(tldpt2**2-epspt2**2),epspl,tldplh,pph
      
c      if(tldpt2.lt.0.d0) return
      if(tldpt2.lt.0.d0.and.(tldpt2**2-epspt2**2).gt.0.d0) return

c       b1=0.d0
c       b2=0.d0
c       b3=0.d0
c       b4=0.d0
c	print *,'q2',tldq2
c Call semi-inclusive model (H_i defined in Mulders)
      
c      dum=tldq2/ap/aks
c      if(tldnu.ne.dum) then
c      print*,'redefine'
c      aks=tldq2/ap/tldnu
c      zh=ehad/tldnu
c      tldsq=sqrt(tldq2+tldnu**2)
c      tldplh=(ehad*tldnu-phq)/tldsq
c      tldpt2=pph**2-tldplh**2
c      if(tldpt2.lt.0.d0) return
c      write(*,'(10f22.19)') tldq2,aks,zh,tldpt,tldnu,Ehad,pph,
c     &dum,(tldq2/ap/tldnu),(zh*tldnu)
c      endif
      
c      write(*,'(10f22.19)') tldq2,aks,zh,tldpt,tldnu,Ehad,pph,
c     &dum,(tldq2/ap/tldnu),(zh*tldnu)
      
c Recover y
      a=s/ap*(s/ap-anu)*tldq2/y
      tlde=(tldnu+sqrt(tldnu**2+4.d0*a))/2.d0
      tldy=tldnu/tlde
      
      call semi_inclusive_model(tldq2,aks,tldy,zh,tldpt2,tldp22,tldplh,H1z,H2z,H3z,H4z)
c       print*,'----------------------------------------------'
       
c      print*,'Hs ',tldq2,aks,zh,tldpt,H1z,H2z,H3z,H4z
	
c	print*,'semi-inclusive'
c	print*,Eb,tldq2,aks,zh,sqrt(tldpt2)
c	print*,H1z,H2z,H3z,H4z
	

c No photon emission (k-->0)
c      aa=sx*(zdif-2.*amp*plh/sqly)/2.d0/amp2
c	h(1)=  zdif*(2.d0*y*xs*sfm0(1)-pth**2*sfm0(4))/amp/y/xs
c	h(2)=2.*zdif*(2.*xs*(xs*sfm0(2)+aa*sfm0(3))*aly
c     . +sfm0(4)*(aa**2*aly-2.*pth**2*y))/amp/xs/y/aly
c        h(3)=2.*sfm0(4)*zdif/amp/y/xs
c	h4=-2.*zdif*(xs*sfm0(3)+aa*sfm0(4))/amp/y/xs
      
c Including kinematic shift due to photon emission
      epsi=ap2/(sx-rr)
      aa=(sx-rr)*(zh-2.d0*amp*tldplh/tldsqly)/2.d0/amp2
      h1=zh*(2.d0*tldq2*aks*h1z-tldpt2*h4z)/amp/tldq2/aks
      h2=2.d0*zh*(2.d0*aks*(aks*h2z+aa*h3z)*tldaly+
     &h4z*(aa**2*tldaly-2.d0*tldpt2*tldq2))/amp/aks/tldq2/tldaly
      h3=2.d0*h4z*zh/amp/tldq2/aks
      h4=-2.d0*zh*(aks*h3z+aa*h4z)/amp/tldq2/aks
      
c      print*,epsi,aa,h1,h2,h3,h4
      

      sfm(1)=un*h1
      sfm(2)=un*h2
      sfm(3)=un*h3
      sfm(4)=un*h4
c      sfm(5)=epsi**2*b1
c      sfm(6)=epsi**3*(b2/3.d0+b3+b4)
c      sfm(7)=epsi*(b2/3.d0-b3)
c      sfm(8)=epsi**2*(b2/3.d0-b4)
      
      
c      print*,'strf: ',tldq2,aks,zh,tldpt2,tldpt,H1z,H2z,H3z,H4z,
c     &tldnu,sx,ap,ehAD
      
      return
      end
