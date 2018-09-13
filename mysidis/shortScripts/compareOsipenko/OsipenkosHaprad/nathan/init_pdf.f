      subroutine init_pdf(g,s)
      implicit none
      double precision VAL(20)
      integer g,s
      character*20 PARM(20)
c-----Init PDFSET
      PARM(1)='Init0'
      VAL(1)=0.d+0
      call PDFSET(PARM,VAL)
c-----Choise of the distribution
      PARM(1)='Nptype'
      VAL(1)=1.d+0
      PARM(2)='Ngroup'
      VAL(2)=float(g)
      PARM(3)='Nset'
      VAL(3)=float(s)
      call PDFSET(PARM,VAL)
      return
      end
