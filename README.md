# Projet : Générateur de Mondes Interactif et Évolutif

Notre projet est un générateur de mondes interactif. L'utilisateur peut d'abord créer une carte personnalisée - biomes, reliefs, océans, civilisations - puis observer ce monde évoluer en temps réel : érosion, végétation, météo, et même le développement de civilisations.

## I. Générateur de Cartes Personnalisées

Cette première phase permet à l'utilisateur de sculpter l'environnement de base selon ses désirs, en définissant des paramètres clés pour façonner la géographie et les conditions initiales du monde.

### A. Génération de Terrain et Environnements

Le système utilisera des algorithmes de génération procédurale avancés (ex: bruit de Perlin, fractales) pour créer des terrains variés et crédibles.

- **Paramètres géographiques et biomes :** L'utilisateur pourra spécifier des proportions ou des zones préférentielles pour différents types de reliefs et biomes :
  - **Reliefs :** Génération de chaînes de montagnes complexes, de plateaux, de vallées fluviales et de plaines.
  - **Hydrologie :** Placement et formation d'océans, de lacs, de rivières et de réseaux hydrographiques cohérents.
  - **Biomes :** Définition de zones climatiques (déserts, forêts tropicales, toundras, taïgas, etc.) conformes à la topographie et à la latitude.
- **Conditions initiales :** Définition des conditions de départ pour la végétation (densité, types d'espèces) et les civilisations (point de départ, ressources initiales, niveau technologique).

### B. Placement de Structures Logiques et Points d'Intérêt

En plus du terrain naturel, l'utilisateur pourra placer des éléments qui serviront de base pour le développement futur :

- **Points de départ :** Définition des emplacements initiaux pour les premières colonies ou villes.
- **Ressources :** Placement de gisements de ressources rares ou stratégiques (minerai, bois, terres fertiles) qui influenceront l'expansion des civilisations.
- **Éléments narratifs :** Possibilité d'intégrer des ruines, des artefacts ou des éléments géographiques uniques pour enrichir l'histoire du monde.

## II. Modification du Terrain et Évolution en Temps Réel

Une fois la carte générée, le monde entre dans une phase d'évolution dynamique et autonome. Des processus naturels et anthropiques modifient continuellement l'environnement.

### A. Processus Naturels et Météorologiques

Des systèmes environnementaux complexes simulent l'impact des forces naturelles :

- **Érosion et Hydrologie :** Simulation de l'érosion du sol causée par les précipitations (pluie, neige) et le ruissellement des eaux, modifiant subtilement les reliefs et créant des deltas ou des canyons au fil du temps.
- **Cycle de la Végétation :** La flore progresse ou régresse en fonction des conditions climatiques et de l'environnement.
  - **Expansion/Régression :** Les forêts s'étendent dans les zones fertiles, ou reculent face à la désertification.
  - **Incendies :** Gestion réaliste des risques d'incendies (causés par la foudre ou l'activité humaine) et de leur propagation.
- **Système Météo Dynamique :** Simulation de cycles météorologiques (saisons, tempêtes, sécheresses) influençant l'agriculture, l'hydrologie et les biomes.

### B. Cadence Temporelle et Vitesse d'Évolution

- **Échelle de Temps Réglable :** L'utilisateur peut ajuster la vitesse à laquelle le temps s'écoule dans le simulateur, permettant d'observer des millénaires d'évolution en quelques minutes ou de zoomer sur des événements en temps quasi réel.
- **Événements Aléatoires :** Introduction d'événements aléatoires (catastrophes naturelles, découvertes archéologiques) pour pimenter le cours du temps.

### C. Évolution des Civilisations

Les entités créées (villages, villes) évoluent de manière organique :

- **Croissance Démographique et Territoriale :** Les populations s'étendent en fonction des ressources disponibles et de la fertilité du terrain.
- **Interactions :** Simulation des relations inter-civilisations (commerce, guerres, alliances) influençant la carte politique et l'expansion urbaine.
- **Développement Technologique :** Les sociétés peuvent progresser de l'âge de pierre à des ères plus avancées (sous réserve des contraintes fixées par l'utilisateur).

## III. Structures et Développement Anthropique

L'évolution du monde est marquée par le développement des structures créées par les civilisations.

### A. Évolution Urbaine

La croissance des colonies est un processus dynamique :

- **Hiérarchie Urbaine :** Les implantations suivent un cycle logique : un campement devient un village, qui peut se développer en ville, puis en cité ou en métropole si les ressources et la stabilité le permettent. Inversement, une cité peut régresser ou être abandonnée.
- **Fonctionnalité Urbaine :** Chaque ville développe des fonctions spécifiques (centre commercial, cité minière, capitale administrative) qui influencent son architecture et son réseau.

### B. Réseaux et Infrastructures

Le commerce et la communication sont essentiels à la prospérité :

- **Réseaux Intra-Muros :** Génération logique de chemins et de rues au sein des zones urbanisées, optimisant les déplacements et l'accès aux ressources.
- **Routes Commerciales et Militaires :** Développement de routes et de ponts connectant les centres de population, créant des axes de développement économique et stratégique. L'emplacement de ces routes est influencé par la topographie (évitement des terrains impraticables).

### C. Génération Procédurale de l'Architecture

L'esthétique des structures s'adapte aux conditions locales :

- **Adaptation au Biome et au Climat :** L'architecture des maisons et des bâtiments publics est générée procéduralement pour refléter le biome et le climat environnants (ex: toits pentus dans les zones enneigées, maisons sur pilotis dans les marécages, matériaux locaux utilisés pour la construction).
- **Reflet de l'Âge et du Statut :** Les bâtiments plus anciens ou appartenant à des classes sociales aisées présenteront des détails architecturaux distinctifs.