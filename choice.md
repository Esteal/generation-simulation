# L'Équation d'Éboulement (Érosion Thermique)

Formule calculant la quantité de matière qui s'effondre :

$$\Delta h = c \cdot (\Delta H_{max} - \tau)$$

*Condition d'activation : L'équation n'est calculée que si* $\Delta H_{max} > \tau$

**Détail des variables :**

* **$\Delta h$ (Matière transférée)** : La quantité exacte de matière (sédiments en priorité, puis roche) qui glisse de la cellule instable vers sa voisine la plus basse lors d'une itération.
* **$c$ (Friction)** : Représente la viscosité du matériau. Elle détermine quelle proportion du surplus instable tombe en une seule étape (souvent autour de `0.5`), ce qui permet d'adoucir la chute et d'éviter de creuser des trous artificiels.
* **$\Delta H_{max}$ (Pente maximale)** : La différence d'altitude la plus forte mesurée entre la cellule étudiée et ses voisines directes plus basses.
* **$\tau$ (Talus Angle / Angle de repos)** : La pente critique maximale tolérée par la géologie locale. Si la pente du terrain est inférieure ou égale à cette valeur, le sol est considéré comme stable et la gravité n'a plus d'effet.

# L'Équation du Stream Power (Érosion Hydraulique)

Formule mathématique :

$$E = K \cdot A^m \cdot S^n$$

**Explications :**

* **$E$ (Stream Power / Puissance du flux)** : La force érosive du courant, correspondant à la quantité de matière (sédiments puis roche) arrachée à la cellule lors d'une itération.
* **$K$ (Constante d'érodabilité)** : La puissance de base de l'érosion. Elle représente la friabilité globale du terrain.
* **$A$ (Aire de drainage / Accumulation)** : Le volume d'eau total traversant la cellule.
* **$m$ (Exposant d'accumulation)** : Paramètre l'impact du volume d'eau, modulant l'avantage des grands fleuves par rapport aux petits ruisseaux.
* **$S$ (Pente / Slope)** : La déclivité locale du terrain (différence d'altitude entre la cellule actuelle et sa voisine la plus basse).
* **$n$ (Exposant de la pente)** : il définit l'importance de la verticalité dans le processus d'érosion.

# Explication du fonctionnement de l'algorithme de création de rivière

## Tâche 1 : Initialisation de la pluie (`initializeWaterMapAndLandCells`)
1. **Étape 1 :** Pour chaque case qui n'est ni un océan ni une plage, on dépose `1.0f` unité d'eau dans une carte virtuelle des eaux (`waterMap`).
2. **Étape 2 :** On enregistre simultanément toutes ces cases terrestres avec leurs coordonnées et leur altitude dans une liste dédiée (`landCells`) de la taille de notre grille.

## Tâche 2 : Tri en fonction de l'altitude (`sortLandCellsByAltitude`)
1. **Étape 1 :** Un simple `sort` de la classe `vector`. Cela va nous permettre de commencer par le traitement des cellules les plus hautes.

## Tâche 3 : Écoulement de l'eau (`routeWater` et `findLowestNeighborIndex`)

1. **Étape 1 :** On prend chaque case terrestre dans l'ordre du tri.
2. **Étape 2 :** On analyse les 8 cases voisines pour trouver celle dont l'altitude est la plus basse (et strictement inférieure à la case actuelle).
3. **Étape 3 :** Si on trouve un voisin plus bas, toute l'eau de la case actuelle (la goutte initiale + l'eau déjà reçue d'autres cases en amont) s'ajoute à ce voisin.

## Tâche 4 : Établissement des rivières (`carveRivers`)
1. **Étape 1 - Application du seuil :** Si une case a accumulé une quantité d'eau supérieure ou égale à la limite fixée (`riverThreshold`), elle devient une "rivière potentielle" via un marqueur booléen (`riverMask` -> tableau de booléens).
2. **Étape 2 - Vérification de connectivité :** L'algorithme supprime les cellules isolées et remet le marqueur à `false`.
3. **Étape 3 - Validation finale :** Les cases ayant passé les deux derniers tests deviennent des rivières.
# Notes Techniques et Spécificités du Projet

## 1. Architecture et Bonnes Pratiques C++

* **Le mot-clé `virtual` (Polymorphisme) :** Indique au compilateur qu'il ne doit pas déterminer quelle méthode appeler en fonction du type de la variable (le pointeur ou la référence), mais en fonction du **type réel** de l'objet en mémoire au moment de l'exécution (résolution dynamique).
* **Encapsulation de la SDL :** Si l'on part sur la bibliothèque SDL, il faudra l'encapsuler dans des classes (concept RAII : *Resource Acquisition Is Initialization*) pour garantir la libération correcte des ressources et éviter les fuites mémoire.
* **Initialisation des objets :** À l'initialisation du jeu, il est préférable d'utiliser directement le **constructeur** de la classe plutôt que de créer une méthode `init()` séparée. Cela garantit qu'un objet instancié est toujours prêt à l'emploi.

## 2. Optimisations des Boucles et de la Mémoire

* **Pré-incrémentation vs Post-incrémentation :** Lors d'une itération, préférez `++x` à `x++`. Avec `x++`, l'ordinateur doit allouer de la mémoire pour stocker une copie temporaire de l'ancienne valeur. Avec `++x`, l'incrémentation se fait directement sans copie, ce qui est très légèrement plus rapide.

* **Parcours d'un tableau 2D aplati (Vector 1D) :** * Lors de l'utilisation de deux boucles `for` imbriquées sur un `vector<T>` représentant une grille 2D, il faut **toujours commencer par la coordonnée `y` (boucle externe) puis `x` (boucle interne)**.
    * *Explication :* Le tableau étant stocké sur un seul bloc de mémoire contigu, il est beaucoup plus simple et rapide pour le processeur de lire les index de manière séquentielle (0, 1, 2, 3...). 
    * Si l'on parcourt d'abord par `x`, le processeur fera d'énormes sauts dans la mémoire (ex: index 0, puis `width`, puis `2 * width`). Cela empêche l'utilisation de la mémoire cache ultra-rapide du processeur (défaut de localité spatiale ou *cache miss*).

## 3. Typage et Structure des Données

* **L'énumération `enum class BiomeIndex : uint8_t` :**
    * `uint8_t` : Force l'énumération à ne prendre qu'un seul octet (8 bits) en mémoire au lieu de 4 (taille standard d'un `int`), ce qui allège considérablement le poids de la grille.
    * `enum class` : Force l'utilisation d'une syntaxe fortement typée (`BiomeIndex::Type`). Cela évite les conflits de noms et les conversions implicites dangereuses, réduisant ainsi les bugs. C'est également la convention standard en C++ moderne.
* **L'utilisation de `size_t` au lieu de `int` :** 
    * À privilégier pour les tailles et les index (comme dans la classe `Grid`).
    * C'est un entier **non signé** (il ne peut pas être négatif, ce qui est logique pour une taille).
    * Sa taille en mémoire s'adapte automatiquement à l'architecture du système (32 ou 64 bits) pour éviter les dépassements de capacité (*overflow*).
    * C'est le type utilisé nativement par la plupart des fonctions de la bibliothèque standard (STL).

* **Préférer `std::unordered_map` à `std::map` pour le gestionnaire de textures :**

    * **Structure sous-jacente :** En C++, une `map` classique est un Arbre Binaire de Recherche, tandis qu'une `unordered_map` est une table de hachage.
    * **Complexité de recherche :** L'arbre binaire nécessite un temps de recherche logarithmique (O(log n)). La table de hachage permet un accès direct en temps constant (O(1)) en moyenne.
    * **Le bénéfice pour le jeu :** Lors de la boucle de rendu, le moteur va demander l'accès aux textures par leur nom (la clé) des dizaines, voire des milliers de fois par frame. Récupérer l'adresse de la texture instantanément via le calcul de son empreinte (le hash de la chaîne de caractères) est fondamentalement plus rapide que de parcourir les branches d'un arbre à chaque appel.
    * **L'inconvénient évité :** L'avantage principal d'une `map` classique est de garder les éléments triés (par ordre alphabétique pour des strings). Comme le moteur de jeu n'a aucun besoin d'itérer sur les textures par ordre alphabétique lors de l'exécution, payer le coût de performance d'un tri permanent serait inutile.

* **L'utilisation du mot-clé `auto` pour les itérateurs (ex: `auto it = textures.find(id);`) :**
    * **Amélioration de la lisibilité :** La méthode `find()` d'une `unordered_map` retourne un objet "itérateur". Sans `auto`, le type exact à écrire serait beaucoup trop long. Par exemple, il faudrait écrire : `unordered_map<std::string, SDL_Texture*>::iterator it = textures.find(id);`. Le mot-clé `auto` demande au compilateur de déduire ce type tout seul, rendant le code plus propre.
    * **Maintenabilité accrue :** Si plus tard dans le développement on décide de changer les types stockés dans la `unordered_map`, l'`auto` s'adaptera dynamiquement lors de la compilation.

## 4. Génération Procédurale (Biomes)


* **Combinaison de 3 bruits de Perlin/Simplex :** Actuellement, le système utilise 3 cartes de bruit différentes. Leur combinaison permet d'obtenir une génération procédurale plus organique avec un aléatoire contrôlé pour déterminer des biomes intéressants.
* **Exemple d'application :** En croisant ces données, pour une altitude et une température données, le niveau d'humidité (le 3ème bruit) permettra de trancher naturellement entre la création d'une jungle (haute humidité) ou d'un désert (faible humidité).

Notes :
sudo apt-get install cloc
cloc --include-lang="C++,C/C++ Header" .

théorie pour l'occlusion ambiante :
1. Pour chaque cellule, on lance des rayons dans Nd​ directions (ex: 8 directions cardinales et diagonales).

2. Pour chaque rayon, on avance pas à pas jusqu'à un rayon maximum R.

3. On calcule l'angle d'élévation α entre la case de départ et la case testée.

4. On conserve l'angle maximum trouvé h(θ).

5. La lumière finale de la case est de 1.0 (pleine lumière) moins la moyenne des ombres générées par les plus hauts sommets dans chaque direction, en appliquant une atténuation W(θ) selon la distance