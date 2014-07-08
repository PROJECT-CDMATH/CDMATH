// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : MED_version.h
//  Author : Vadim SANDLER
//  Module : SALOME
//
#if !defined(__MED_VERSION_H__)
#define __MED_VERSION_H__

/*
  MED_VERSION is (major << 16) + (minor << 8) + patch.
*/

// MED_VERSION is reserved enum of MEDMEM,
// thus we use alternative constant name for SALOME MED module
#define SALOMEMED_VERSION_STR "7.4.0"
#define SALOMEMED_VERSION     0x070400
#define SALOMEMED_DEVELOPMENT 0

#endif // __MED_VERSION_H__
