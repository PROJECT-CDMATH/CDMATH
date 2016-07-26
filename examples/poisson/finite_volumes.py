#!/usr/bin/env python
# -*-coding:utf-8 -*

#===============================================================================
# Name       : Résolution VF de l'équation de Poisson -Δu = f
#                  avec conditions aux limites de Dirichlet u=0.
# Author     : Michaël Ndjinga
# Copyright  : CEA Saclay 2016
# Description: Utilisation de la méthode des volumes finis
#                  avec champs u et f discrétisés aux cellules
#                  d'un maillage quelconque.
#                  Création et sauvegarde du champ résultant ainsi que
#                  du champ second membre en utilisant CDMATH.
#===============================================================================

from math import pi
from math import sin

import cdmath

# Création d'un maillage cartésien du domaine carré [0,1]x[0,1], définition des bords
#====================================================================================
xmin = 0
xmax = 1
ymin = 0
ymax = 1
#zmin = 0 si calcul 3D
#zmax = 1 si calcul 3D
nx = 51
ny = 51
#nz = 50 si calcul 3D
my_mesh = cdmath.Mesh(xmin,xmax,nx,ymin,ymax,ny)#Ajouter ,zmin,zmax,nz si calcul 3D
eps = 1e-6
my_mesh.setGroupAtPlan(0,0,eps,"DirichletBorder")#Bord GAUCHE
my_mesh.setGroupAtPlan(1,0,eps,"DirichletBorder")#Bord DROIT
my_mesh.setGroupAtPlan(0,1,eps,"DirichletBorder")#Bord BAS
my_mesh.setGroupAtPlan(1,1,eps,"DirichletBorder")#Bord HAUT
#my_mesh.setGroupAtPlan(0,2,eps,"DirichletBorder")#Bord AVANT si calcul 3D
#my_mesh.setGroupAtPlan(1,2,eps,"DirichletBorder")#Bord ARRIERE si calcul 3D

nbCells = my_mesh.getNumberOfCells()

print("Fin construction du maillage")
print("nb of cells ",nx,"x",ny, " = ", nbCells)

# Discrétisation du second membre et détermination des noeuds intérieurs
#=======================================================================
my_RHS_field = cdmath.Field("RHS field", cdmath.CELLS, my_mesh, 1)
maxNbNeighbours = 0#This is to determine the number of non zero coefficients in the sparse finite element rigidity matrix
#parcours des cellules pour discrétisation du second membre et extraction du nb max de voisins d'une cellule
for i in range(nbCells):
    Ci = my_mesh.getCell(i)
    x = Ci.x()
    y = Ci.y()
    # z = Ci.z() si calcul 3D
    my_RHS_field[i] = 2*pi*pi*sin(pi*x)*sin(pi*y)#mettre la fonction definie au second membre de l edp
    # compute maximum number of neighbours
    maxNbNeighbours = max(1+Ci.getNumberOfFaces(),maxNbNeighbours)

# sauvegarde sur le disque dur du second membre discrétisé dans un fichier paraview
my_RHS_field.writeVTK("FiniteVolumesRHSField")

print("Fin de la discrétisation du second membdre de l'edp")
print("Max nb of neighbours = ", maxNbNeighbours)

# Construction de la matrice de rigidité et du vecteur second membre du système linéaire
#=======================================================================================
rigidite = cdmath.SparseMatrix(nbCells,nbCells,nbCells*maxNbNeighbours)
RHS = cdmath.Vector(nbCells)
#Parcours des cellules du domaine
for i in range(nbCells):
    RHS[i] = my_RHS_field[i] #la valeur moyenne du second membre f dans la cellule i
    Ci = my_mesh.getCell(i)
    for j in range(Ci.getNumberOfFaces()):# parcours des faces voisinnes
        Fj = my_mesh.getFace(Ci.getFaceId(j))
        coeff = Fj.getMeasure()/Ci.getMeasure()
        if not Fj.isBorder():
            k = Fj.getCellId(0)
            if k == i :
                k = Fj.getCellId(1)
            Ck = my_mesh.getCell(k)
            distance = Ci.getBarryCenter().distance(Ck.getBarryCenter())
            rigidite.setValue(i,k,-coeff/distance) # terme extradiagonal
        coeff = coeff/2/Ci.getBarryCenter().distance(Fj.getBarryCenter()) + rigidite(i,i) #on ajoute la contribution sur la diagonale
        rigidite.setValue(i,i,coeff) # terme diagonal

print("Fin du remplissage matrice de rigidité")

# Résolution du système linéaire
#=================================
LS = cdmath.LinearSolver(rigidite,RHS,500,1.E-6,"CG","ILU")
SolSyst = LS.solve()

print("Fin de la résolution du système linéaire")

# Création du champ résultat
#===========================
my_result_field = cdmath.Field("Result field", cdmath.CELLS, my_mesh, 1)
for i in range(nbCells):
    my_result_field[i] = SolSyst[i];
#sauvegarde sur le disque dur du résultat dans un fichier paraview
my_result_field.writeVTK("FiniteVolumesResultField")

print("Fin de la résolution numérique VF du problème de poisson 2D")

#Calcul de l'erreur commise par rapport à la solution exacte
#===========================================================
max_sol_exacte = (my_RHS_field.getNormEuclidean()).max()/(2*pi*pi)
erreur_max = (my_RHS_field/(2*pi*pi) - my_result_field).getNormEuclidean().max()
print("max(| solution numerique - solution exacte |)/max(| solution exacte |) = ",erreur_max/max_sol_exacte)

#Postprocessing optionnel: ouverture du fichier FiniteElementsResultField.pvd contenant le résultat numérique à partir de commandes python (import paraview)
