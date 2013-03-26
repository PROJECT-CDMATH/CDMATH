"nvaluesperentity=Nombre de valeurs par entité." \
"nvaluesperentityMEDfilterCm=Utiliser la valeur 1 pour un filtre d'éléments de maillage." \
"nvaluesperentityMEDfilterEx=Cela peut être le nombre de points d'intégration utilisé dans un champ résultat." \
\
"nconstituentpervalue=Nombre de constituants par valeur." \
"nconstituentpervalueMEDfilterEx=Cela peut être le nombre de coordonnées des noeuds,\
 le nombre de noeuds d'une connectivité, le nombre de composantes d'un champ résultat." \
\
"constituentselect=Numéro de constituant des valeurs à filtrer (commence à 1).\
 Le mot clé #MED_ALL_CONSTITUENT permet de selectionner tous les constituants." \
"constituentselectMEDfilterEx=Cela peut être le numéro de coordonnées des noeuds, le numéro de noeuds d'une connectivité,\
 le numéro de composantes d'un champ résultat." \
\
"filterarraysize=Nombre d'entités à filtrer et taille du tableau \a filterarray." \
"filterarraysizeMEDfilterCm=Ne prend pas en compte \a nvaluesperentity et \a nconstituentpervalue." \
\
"filterarray=Tableau d'index du profil des numéros d'entités associées aux valeurs à sélectionner." \
\
"filter=Filtre sur entités (\ref med_filter) appliqué en lecture/écriture de valeurs." \
"filterMEDfilterCm=La déallocation est à la charge de l'utilisateur (cf. \ref MEDfilterClose \ref MEDfilterDeAllocate)." \
"nentityMEDfilterCm=Ce paramètre ne prend pas en compte \a nvaluesperentity et \a nconstituentpervalue." \
\
"start=Index de départ du premier bloc. L'index est relatif à un profil." \
"stride=Ecart constant en nombre d'entités entre les blocs (en comptant à partir de l'indice de départ du bloc précédent). \
 En particulier, écart entre \a start et l'index de départ du bloc suivant." \
"count=Nombre de blocs à sélectionner (0 est possible)." \
"blocksize=Taille de chaque bloc en nombre d'entités." \
"lastblocksize=Taille en nombre d'entités du dernier bloc du dernier processus utilisé (inutilisé si \a count < 2 )." \
"nfilter=Nombre de filtres." \
\
\
\
"MEDfilterBrief=Crée un filtre élémentaire en selectionnant les entités pour lesquelles on veut lire/écrire des valeurs. \
Le tableau \a filterarray contient des index croissants d'un tableau de profil. \
Cette sélection permet une lecture/écriture avancée vers/depuis les emplacements mémoire sélectionnés. \
Elle s'utilise uniquement <b>en mode séquentiel</b> (un seul processus)." \
\
"MEDfilterDetails=La sélection des emplacements mémoire lus/écrits s'opère dans l'espace mémoire d'un seul processus.\
 Les entités sélectionnées le sont avec toutes leurs valeurs (\a nvaluesperentity).\
 Il est possible de sélectionner l'ensemble des constituants par valeur ( #MED_ALL_CONSTITUENT )\
 ou uniquement les constituants n° \a constituentselect . \
 Les constituants sont lus/écrits en mode #MED_FULL_INTERLACE|#MED_NO_INTERLACE (\ref med_switch_mode).\
 Le mode de stockage peut être #MED_GLOBAL ou #MED_COMPACT (\ref med_storage_mode).\
 Les index définis dans le tableau de filtre (\a filterarray) sont des index relatifs à un tableau profil\
 (eventuellement le tableau virtuel #MED_ALLENTITIES_PROFILE ou #MED_NO_PROFILE ).\
 La numérotation des index commence à 1.\
 Les index se définissent de la façon suivante :\
 \li Si le filtre opère sur un tableau de valeurs sans profil,\
 les index sont les numéros d'entités utilisées selon la numérotation MED relative au type géométrique de l'entité\
 (équivaut aux index d'un profil #MED_ALLENTITIES_PROFILE).\
 \li Si le filtre opère sur un tableau de valeurs avec profil en mode #MED_GLOBAL,\
 les index du tableau \a filterarray indiquent les numéros d'entités à selectionner dans le tableau \a profil.\
 \li Si le filtre opère sur un tableau de valeurs avec profil en mode #MED_COMPACT,\
 les index du tableau \a filterarray indiquent les numéros d'entités à selectionner dans le tableau \a profil." \
 \
"MEDfilterNote=Le filtre crée prend en compte les paramètres \a nvaluesperentity et \a nconstituentpervalue\
 pour selectionner les emplacements en mémoire en fonction du mode de stockage utilisé (\ref med_storage_mode)." \
\
\
\
"MEDfilterBlockOfEntityBrief=Crée un filtre en selectionnant par blocs les entités\
 pour lesquelles on veut lire/écrire des valeurs.\
 Cette sélection permet une lecture/écriture avancée vers/depuis les emplacements mémoire sélectionnés.\
 Elle s'utilise aussi bien en mode séquentiel qu'en mode parallèle (un ou plusieurs processus)." \
\
"MEDfilterBlockOfEntityDetails=La sélection des emplacements mémoire lus/écrits s'opère dans l'espace\
 mémoire du processus appelant.\
 Les entités sélectionnées le sont avec toutes leurs valeurs (\a nvaluesperentity).\
 Il est possible de sélectionner l'ensemble des constituants par valeur ( #MED_ALL_CONSTITUENT )\
 ou uniquement les constituants n°\a constituentselect . \
 Les constituants sont lus/écrits en mode #MED_FULL_INTERLACE|#MED_NO_INTERLACE (\ref med_switch_mode).\
 Le mode de stockage peut être #MED_GLOBAL ou #MED_COMPACT (\ref med_storage_mode) :\
 \li En mode #MED_GLOBAL, chacun des processus possède l'espace mémoire des valeurs associés à la totalité des entités d'un même type.\
 Dans ce mode, il n'est pas possible d'utiliser un profil utilisateur (car la sélection ne serait plus par bloc).\
 Les seuls profils disponibles sont #MED_ALLENTITIES_PROFILE ou #MED_NO_PROFILE .\
 \li En mode #MED_COMPACT, chacun des processus possède un espace mémoire contigü pour lire/écrire\
 les valeurs des entités selectionnées par des blocks d'index.\
 Les blocks décrivent les parties du profil à utiliser.\
 Autrement dit, les blocks d'index créés indiquent les numéros d'entités à selectionner dans le tableau \a profil.\
 La numérotation des index commence à 1.\
 Il est possible d'utiliser un profil utilisateur ou #MED_ALLENTITIES_PROFILE.\
 Si le profil #MED_ALLENTITIES_PROFILE est utilisé, les index sont directement les numéros des entités." \
\
"MEDfilterBlockOfEntityNote=\
 \li Le filtre créé prend en compte les paramètres \a nvaluesperentity et \a nconstituentpervalue\
 pour sélectionner les emplacements en mémoire.\
 \li Si \a count vaut 0 ou 1, \a lastblocksize n'est pas utilisé, le paramètre \a blocksize peut avoir des valeurs différentes par processus.\
 \li Si \a count > 2, chaque processus sélectionne \a count blocks de taille \a blocksize. En utilisant le \a stride\
 il est possible de mettre en oeuvre une sélection par block cyclique. Seul le processus de plus haut rang utilisé\
 (dernier processus utilisé) peut définir une valeur non nulle de \a lastblocksize. Ce paramètre vaudra alors \a blocksize auquel\
 il sera ajouté un résidu non nul. Par exemple : lastblocksize= blocksize + (nentities - blocksize*nproc*nblocks_pproc).\
 \li Il est possible de ne pas faire participer certains processus en indiquant un \a count == 0. Cependant tous les processus du\
 communicateur utilisé doivent faire appel aux même fonctions MED. Il s'agit d'opérations collectives." \
\
\
"MEDfilterAllocateBrief=Alloue un tableau de filtres de taille \a nfilter." \
"MEDfilterAllocateDetails=" \
"MEDfilterAllocateNote=\li Ne pas oublier d'appeler \ref MEDfilterDeAllocate pour libérer les ressources hdf et la mémoire utilisée. \
\li En C, il est possible de déclarer directement une variable de type \ref med_filter que l'on initialise à #MED_FILTER_INIT. \
Il sera alors necessaire d'appeler  \ref MEDfilterClose pour libérer correctement les ressources détenues par le filtre." \
\
\
"MEDfilterDeAllocateBrief=Desalloue un tableau de filtre de taille \a nfilter." \
"MEDfilterDeAllocateDetails=" \
"MEDfilterDeAllocateNote=MEDfilterDeAllocate appelle \ref MEDfilterClose." \
\
\
"MEDfilterCloseBrief=Désalloue les ressources hdf détenues par un filtre." \
"MEDfilterCloseDetails=" \
"MEDfilterCloseNote=MEDfilterClose est surtout destinée à être appelée en C, pour une variable de type \ref med_filter déclarée directement et initialisée à #MED_FILTER_INIT." \
\
\
\
\
\
\
"MEDfilterNoIGNote=Pour cette fonction, la sélection s'opère dans un espace mémoire ou les éléments \
sont stockés uniquement en mode #MED_NO_INTERLACE et #MED_GLOBAL_PFLMODE." \
\
"MEDfilterFullIGNote=Pour cette fonction, la sélection s'opère dans un espace mémoire ou les éléments \
sont stockés uniquement en mode #MED_FULL_INTERLACE et #MED_GLOBAL_PFLMODE." \
\
"MEDfilterNoICNote=Pour cette fonction, la sélection s'opère dans un espace mémoire ou les éléments \
sont stockés uniquement en mode #MED_NO_INTERLACE et #MED_COMPACT_PFLMODE." \
\
"MEDfilterFullIGNote=Pour cette fonction, la sélection s'opère dans un espace mémoire ou les éléments \
sont stockés uniquement en mode #MED_FULL_INTERLACE et #MED_COMPACT_PFLMODE." \