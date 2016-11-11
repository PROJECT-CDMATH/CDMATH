C*  This file is part of MED.
C*
C*  COPYRIGHT (C) 1999 - 2016  EDF R&D, CEA/DEN
C*  MED is free software: you can redistribute it and/or modify
C*  it under the terms of the GNU Lesser General Public License as published by
C*  the Free Software Foundation, either version 3 of the License, or
C*  (at your option) any later version.
C*
C*  MED is distributed in the hope that it will be useful,
C*  but WITHOUT ANY WARRANTY; without even the implied warranty of
C*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C*  GNU Lesser General Public License for more details.
C*
C*  You should have received a copy of the GNU Lesser General Public License
C*  along with MED.  If not, see <http://www.gnu.org/licenses/>.
C*


      subroutine mfiope(fid, name, access, cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfiope
c
      implicit none
      save
      character *(*) name
      integer fid
      integer access,cret
      integer mfifope
c
      fid = mfifope(name, access, len(name))
      if (fid.lt.0) then
         cret=-1
      else
         cret=0
      endif
c      
      return
      end



      subroutine mfinam(fid, name, cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfinam
c
      implicit none
      save
      character *(*) name
      integer fid
      integer namesize,cret
      integer mfifnam
c
      cret = mfifnam(fid, name, len(name))
c      
      return
      end



      subroutine mficlo(fid, cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mficlo
c
      implicit none
      save
c
      integer fid,cret
      integer mfifclo
c
      cret = mfifclo(fid)
c      
      return
      end



      subroutine mficow(fid,cmt,cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mficow
c
      implicit none
      save
c
      integer fid,cret
      character*(*) cmt
      integer mfifcow
c
      cret = mfifcow(fid,cmt,len(cmt),cret)
c      
      return
      end


      subroutine mficor(fid,cmt,cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mficor 
c
      implicit none
      save
c
      integer fid,cret
      character*(*) cmt
      integer mfifcor
c
      cret = mfifcor(fid,cmt,len(cmt),cret)
c      
      return
      end



      subroutine  mfinvr(fid,major,minor,rel,cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfinvr
c
      implicit none
      save
c
      integer fid,major, minor,rel
      integer cret
      integer mfifnvr
c
      cret = mfifnvr(fid,major,minor,rel)
c      
      return
      end


      subroutine mfisvr(fid,version,cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfisvr
c
      implicit none
      save
c
      character*(*) version
      integer cret,fid
      integer mfifsvr
c
      cret =  mfifsvr(fid,version,len(version))
c      
      return
      end

c
c
c
      subroutine mficom(fname,hdfok,medok,cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mficom
c
      implicit none
      save
c
      integer cret
      character *(*) fname
      integer hdfok,medok
      integer mfifcom
c     
      cret =  mfifcom(fname,len(fname),hdfok,medok)
c
      return
      end


      subroutine mfiomn(fid, fname, class, mid, cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfiomn
c
      implicit none
      save
c
      character *(*) fname
      integer class, fid, mid , cret
      integer mfifomn
c
      mid =  mfifomn(fid, fname, len(fname), class)
      if (mid .eq.-1) then
         cret=-1
      else
         cret=0
      endif
c      
      return
      end

c
c
c
      subroutine mfioun(fid, mid, class, cret)
c     DEC$ ATTRIBUTES DLLEXPORT :: mfioun
c
      implicit none
      save
c
      integer mid, fid, class, cret
      integer mfifoun
c
      cret = mfifoun(fid, mid, class)
c      
      return
      end
