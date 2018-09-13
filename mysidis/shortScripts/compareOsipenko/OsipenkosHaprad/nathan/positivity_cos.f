      subroutine positivity_cos(cos_phi,cos_2phi)
      implicit none
      double precision cos_phi,cos_2phi
      double precision frac
      data frac/0.95d0/
c Loop until all inequalities are satisfied
123   if((1.d0+2.d0*cos_phi+2.d0*cos_2phi).lt.0.d0
     &.or.(1.d0-2.d0*cos_phi+2.d0*cos_2phi).lt.0.d0
     &.or.(abs(cos_2phi).gt.0.d0
     &.and.(1.d0+0.75d0*cos_phi**2/cos_2phi-2.d0*cos_2phi).lt.0.d0)) then
c Extreme I (phi=0 or 2pi)
      if((1.d0+2.d0*cos_phi+2.d0*cos_2phi).lt.0.d0) then
      if(cos_phi*cos_2phi.gt.0.d0) then
      if(abs(cos_phi).gt.abs(cos_2phi)) then
      cos_phi=(-cos_2phi-0.5d0)*frac
      else
      cos_2phi=(-cos_phi-0.5d0)*frac
      endif
      else
      if(cos_phi.lt.0.d0) then
      cos_phi=(-cos_2phi-0.5d0)*frac
      else
      cos_2phi=(-cos_phi-0.5d0)*frac
      endif
      endif
      endif
c Extreme II (phi=pi)
      if((1.d0-2.d0*cos_phi+2.d0*cos_2phi).lt.0.d0) then
      if(cos_phi*cos_2phi.lt.0.d0) then
      if(abs(cos_phi).gt.abs(cos_2phi)) then
      cos_phi=(cos_2phi+0.5d0)*frac
      else
      cos_2phi=(cos_phi-0.5d0)*frac
      endif
      else
      if(cos_phi.gt.0.d0) then
      cos_phi=(cos_2phi+0.5d0)*frac
      else
      cos_2phi=(cos_phi-0.5d0)*frac
      endif
      endif
      endif
c Extreme III (phi=pi+-alpha)
      if(abs(cos_2phi).gt.0.d0) then
      if((1.d0+0.75d0*cos_phi**2/cos_2phi-2.d0*cos_2phi).lt.0.d0) then
      if(cos_2phi.gt.0.d0) then
      cos_2phi=((1.d0+sqrt(1.d0+6.d0*cos_phi**2))/4.d0)*frac
      else
      cos_2phi=((1.d0-sqrt(1.d0+6.d0*cos_phi**2))/4.d0)/frac ! Increase abs value
      endif
      endif
      endif
      goto 123
      endif ! Main branch
      return
      end

