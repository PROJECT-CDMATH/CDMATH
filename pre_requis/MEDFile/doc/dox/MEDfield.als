"fieldname=Nom du champ dans le fichier, de longueur maximum #MED_NAME_SIZE ." \
"csit=Itérateur sur le numéro de séquence de calcul. L'itérateur commence à 1." \
"numdt=Numéro de pas de temps de la séquence de calcul (#MED_NO_DT si pas de pas de temps)." \
"numit=Numéro d'itération de la séquence de calcul (#MED_NO_IT si pas de numéro d'itération)." \
"meshnumdt=Numéro de pas de temps de la séquence de calcul du maillage associé (#MED_NO_DT si pas de pas de temps)." \
"meshnumit=Numéro d'itération de la séquence de calcul du maillage associé  (#MED_NO_IT si pas de numéro d'itération)." \
"dt=Date du pas de temps si le numéro de pas de temps est différent de #MED_NO_DT." \
"fieldtype=Type numérique des composantes du champ." \
"ncomponent=Nombre de composantes." \
"componentname=Nom des composantes du champ. Le nom d'une composante est défini sur \a nbcomponent * #MED_SNAME_SIZE caractères." \
"ind=Itérateur. L'itérateur commence à 1." \ 
"componentunit=Unité des composantes du champ. Le nom de l'unité d'une composante est défini sur #MED_SNAME_SIZE caractères." \
"dtunit=Unité des pas de temps associés aux séquences de calcul du champ, définie dans une chaîne de taille #MED_SNAME_SIZE ." \
"localmesh=Indicateur de localisation du maillage : #MED_TRUE si le maillage est dans le même fichier que le champ, #MED_FALSE si le maillage est dans un autre fichier." \
"ncstp=Nombre de séquences de calcul dans le champ." \
"value=Tableau des valeurs." \
"nvalue=de valeurs." \
"nfield=de champs." \
"ncomponent=de composantes." \
"nprofile=de profils." \
"defaultlocalizationname=Nom de fonction de localisation par défaut, de longueur maximum #MED_NAME_SIZE , (#MED_NO_LOCALIZATION si pas de fonction de localisation)". \
"nintegrationpoint=Nombre de points d'intégation (1 par défaut)" \ 
"localizationname=Nom de fonction de localisation, de longueur maximum #MED_NAME_SIZE , (#MED_NO_LOCALIZATION si pas de fonction de localisation)". \
"MEDfieldComputingStepInfoBrief=Cette routine permet de lire les informations caractérisant une séquence de calcul : pas de temps, numéro d'ordre." \
"MEDfieldComputingStepInfoDetails=Cette routine permet de lire les informations caractérisant une séquence de calcul : pas de temps, numéro d'ordre. Une fois le nombre d'étapes de calcul connu, il est possible de lire les informations caractérisant chaque étape par itération sur l'itérateur de séquence de calcul. Une séquence de calcul est identifiée par un couple : \
\li numéro de pas de temps (#MED_NO_DT si pas de pas de temps)\
\li numéro d'itération (#MED_NO_IT si pas de numéro d'itération)." \
"MEDfieldComputingStepMeshInfoBrief=\MEDfieldComputingStepInfoBrief Elle indique également la séquence de calcul a utiliser pour le maillage associé." \ 
"MEDfieldComputingStepMeshInfoDetails=\MEDfieldComputingStepMeshInfoBrief \
\li numéro de pas de temps (#MED_NO_DT si pas de pas de temps)\
\li numéro d'itération (#MED_NO_IT si pas de numéro d'itération)." \
"MEDfieldComputingStepMeshWrBrief=Cette routine permet de définir la séquence de calcul  ( \a meshnumdit , \a meshnumit ) à utiliser pour le maillage \
associé au champ résultat à la séquence de calcul ( \a numdit , \a numit )." \ 
"MEDfieldComputingStepMeshWrDetails=\MEDfieldComputingStepMeshWrBrief \
\li numéro de pas de temps (#MED_NO_DT si pas de pas de temps)\
\li numéro d'itération (#MED_NO_IT si pas de numéro d'itération)." \
"MEDfieldCrBrief=Cette routine permet de créer un champ dans un fichier." \
"MEDfieldCrDetails=Cette routine permet de créer un champ dans un fichier. Un champ est composé d'une ou plusieurs composantes scalaires. Chaque composante se voit attribuer un nom et une unité. Le type des valeurs des composantes peut être au choix (\ref med_field_type) : \
\li #MED_FLOAT64 : flottant 64 bits, \
\li #MED_INT32 : entier 32 bits, \
\li #MED_INT64 : entier 64 bits." \
"MEDfieldInfoBrief=Cette routine permet de lire les informations d'indentification d'un champ." \
"MEDfieldInfoDetails=Cette routine permet de lire les informations d'indentification d'un champ, les informations lues sont : \
\li Nom du champ, \
\li Nom du maillage associé, \
\li Localisation du maillage : dans le même fichier ou non (\ref med_bool ), \
\li Type des valeurs des composantes du champ (\ref med_field_type ), \
\li Nom et unité des composantes, \ 
\li Unité des pas de temps, \
\li Nombre de séquences de calcul." \
"MEDfieldInfoByNameBrief=Cette routine permet de lire les informations d'indentification d'un champ par accès direct (nom du champ connu)." \
"MEDfieldInfoByNameDetails=Cette routine permet de lire les informations d'indentification d'un champ par accès direct (nom du champ connu), les informations lues sont : \
\li Nom du maillage associé, \
\li Localisation du maillage : dans le même fichier ou non (\ref med_bool ), \
\li Type des valeurs des composantes du champ (\ref med_field_type ), \
\li Nom et unité des composantes, \ 
\li Unité des pas de temps, \
\li Nombre de séquences de calcul." \
"MEDfieldValueAdvancedRdBrief=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et selon un filtre donnés." \
"MEDfieldValueAdvancedRdDetails=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et selon un filtre donnés. Cette routine est une routine dite avancée car le paramètre correspondant au filtre permet de sélectionner finement les données lues en mode séquentiel ou parallèle : avec ou sans profil, mode d'entrelacement, par blocs, etc. " \
"MEDfieldValueAdvancedWrBrief=Cette routine permet d'écire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et selon un filtre donnés." \
"MEDfieldValueAdvancedWrDetails=Cette routine permet d'écrire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et selon un filtre donnés. Cette routine est une routine dite avancée car le paramètre correspondant au filtre permet de sélectionner finement les données lues en mode séquentiel ou parallèle : avec ou sans profil, mode d'entrelacement, par blocs, etc. " \
"MEDfieldValueRdBrief=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul donnée (pas de gestion de profil)." \
"MEDfieldValueRdDetails=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul donnée (pas de gestion de profil)." \
"MEDfieldValueWrBrief=Cette routine permet d'écrire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul donnée (pas de gestion de profil)." \
"MEDfieldValueWrDetails=Cette routine permet d'écrire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul donnée (pas de gestion de profil)." \
"MEDfieldValueWithProfileRdBrief=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et un profil donnés." \
"MEDfieldValueWithProfileRdDetails=Cette routine permet de lire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et un profil donnés. Le profil est identifié par un nom et le mode de stockage des données en mémoire peut être paramétré : compact ou global." \
"MEDfieldValueWithProfileWrBrief=Cette routine permet d'écrire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et un profil donnés." \
"MEDfieldValueWithProfileWrDetails=Cette routine permet d'écrire les valeurs d'un champ définies sur des entités d'un maillage pour une séquence de calcul et un profil donnés. Le profil est identifié par un nom et le mode de stockage des données en mémoire peut être paramétré : compact ou global." \
"MEDfieldnComponentBrief=Cette routine lit le nombre de composantes d'un champ." \
"MEDfieldnComponentDetails=Cette routine lit le nombre de composantes d'un champ. L'indice correspond à l'indice du champ dans le fichier." \
"MEDfieldnComponentByNameBrief=Cette routine lit le nombre de composantes d'un champ (accès direct à partir du nom du champ)." \
"MEDfieldnComponentByNameDetails=Cette routine lit le nombre de composantes d'un champ. L'accès direct au champ se fait à partir de son nom." \
"MEDfieldnProfileBrief=Cette routine permet de lire le nombre de profils référencés dans un champ pour une séquence de calcul, et un type d'entité donnés." \
"MEDfieldnProfileDetails=Cette routine permet de lire le nombre de profils référencés dans un champ pour une séquence de calcul, et un type d'entité donnés. Si un seul nom de profil et un seul nom de localisation d'intégration sont présents, on accède directement à ces noms par l'intermédiaire des deux noms par défaut qui sont renvoyés." \
"MEDfieldnValueBrief=Cette routine permet de lire le nombre de valeurs dans un champ pour une séquence de calcul, et un type d'entité donnés (pas de gestion des profils)." \
"MEDfieldnValueDetails=Cette routine permet de lire le nombre de valeurs dans un champ pour une séquence de calcul, et un type d'entité donnés (pas de gestion des profils). Ce nombre de valeurs permet de calculer la zône mémoire à allouer en vue de lire ces données (à savoir le nombre de valeurs * nombre de composantes du champ)." \
"MEDfieldnValueWithProfileBrief=Cette routine permet de lire le nombre de valeurs à lire dans un champ pour une séquence de calcul, et un type d'entité donnés pour un profil donné." \
"MEDfieldnValueWithProfileDetails=Cette routine permet de lire le nombre de valeurs à lire dans un champ pour une séquence de calcul, et un type d'entité donnés selon un profil donné. Ce nombre de valeurs permet de calculer la zône mémoire à allouer en vue de lire ces données (à savoir le nombre de valeurs * nombre de composantes du champ * nombre de point d'integration)." \
"MEDfieldnValueWithProfileByNameBrief=Cette routine permet de lire le nombre de valeurs à lire dans un champ pour une séquence de calcul, et un type d'entité donnés pour un profil donné (accès direct au champ via son nom)." \
"MEDfieldnValueWithProfileByNameDetails=Cette routine permet de lire le nombre de valeurs à lire dans un champ pour une séquence de calcul, et un type d'entité donnés selon un profil donné (accès direct au champ via son nom). Ce nombre de valeurs permet de calculer la zône mémoire à allouer en vue de lire ces données (à savoir le nombre de valeurs * nombre de composantes du champ * nombre de point d'integration)." \
"MEDnFieldBrief=Cette routine permet de lire le nombre de champs dans un fichier." \
"MEDnFieldDetails=Cette routine permet de lire le nombre de champs dans un fichier." \
