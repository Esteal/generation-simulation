# Cahier des charges – Générateur de Mondes Interactif et Évolutif

## Chapitre 1 – Présentation du projet

### Contexte

Nous avons choisi comme projet de développer un générateur de mondes interactif. L'utilisateur peut dans un premier temps créer une carte personnalisée - biomes, reliefs, océans, structures - puis observer l’évolution dynamique du monde à travers des phénomènes naturels et le développement de civilisations.

L'idée initiale du projet s'inspire du projet de génération de carte indépendant Canvas of Kings, et vient ensuite se combiner à d'autres inspirations personnelles.


### Historique

Le projet s’inscrit dans la continuité de travaux réalisés durant notre cursus en licence d'informatique, notammment autour de :

- simulations physiques (LIFAMI)
- génération procédurale et les algorythmes de bruit via nos projets
academiques (LIFAMI)
- programmation en C++ (LIFAPI,LIFAMI,LIFAPSD,LIFAPCD)

L’objectif est de concevoir un système plus complexe combinant plusieurs types de simulation dans un même environnement.

---

## Chapitre 2 – Description de la demande

### Objectifs du projet

Le projet doit permettre de :

- générer automatiquement un terrain virtuel crédible
- permettre à l’utilisateur de personnaliser la carte
- simuler l’évolution naturelle du monde
- simuler le développement de civilisations

L’utilisateur doit pouvoir observer ces évolutions en temps réel ou accéléré.

### Fonctionnalités du produit

#### 1. Génération du terrain

Le système peut générer automatiquement un terrain crédible dans sa structure, intégrant du relief, différents matériaux, etc.

Cette génération repose sur des algorithmes procéduraux.

Le monde doit contenir différents environnements (biomes) qui sont déterminés en fonction de la température, de l’humidité et de l’altitude.

#### 2. Simulation environnementale

Le système peut simuler :

- érosion du terrain
- croissance ou disparition de la végétation

Ces phénomènes modifient progressivement la carte.

#### 3. Gestion des structures et simulation urbaine

Le programme doit permettre :

- l'implémentation de structures sur le terrain par l'utilisateur
- le developpement de villages et de villes 
- l'évolution des structures créées dans le temps

---

## Chapitre 3 – Contraintes

### Contraintes techniques

Le projet doit :

- être développé en langage C++
- être composé d'un noyau de code indépendant de l'affichage

Le programme doit donc pouvoir s'adapter à plusieurs bibliothèque d'affichage sans apporter de modification majeure au noyau.

### Contraintes temporelles

La durée de développement est limitée à la durée du projet académique.

Le projet doit être réalisé en plusieurs étapes afin d’obtenir un prototype fonctionnel rapidement.

### Contraintes de complexité

En raison des limites de temps et de nos connaissances respectives:

- certaines simulations seront simplifiées
- la modélisation de certains élements sera simplifiée

---
## Chapitre 4 – Déroulement du projet

Le projet académique est prévu pour une durée totale de 8 semaines. Il implique une équipe de 2 développeurs et se divise en plusieurs phases.

### Phase 1 – Conception

#### T1 : Architecture & Diagramme de classes
* **Développeurs assignés :** Dev 1 & 2
* **Description :** Définir la structure globale du programme (représentation du monde, structures de données, organisation des modules).
* **Livrable :** Diagramme des classes représentant l’architecture du programme.

---

### Phase 2 – Génération du monde

#### T2 : Génération du terrain (altitude, matériaux)
* **Développeur assigné :** Dev 1
* **Description :** Implémenter l’algorithme de génération procédurale pour créer la carte en gérant l'altitude et les couches de matériaux.
* **Livrable :** Code de génération du terrain.

#### T2b : Gestion de l'affichage
* **Développeurs assignés :** Dev 1 & 2
* **Description :** Interfacer le noyau de génération avec la bibliothèque graphique pour permettre la visualisation de la carte.
* **Livrable :** Rendu visuel du terrain généré.

#### T3 : Génération des biomes
* **Développeur assigné :** Dev 2
* **Description :** Améliorer la génération du terrain pour la rendre plus crédible et associer à chaque zone de la carte un biome.
* **Livrable :** Carte avec différents biomes visibles.

---

### Phase 3 – Simulation environnementale

#### T4 : Érosion, végétation, météo
* **Développeurs assignés :** Dev 1 & 2
* **Description :** Implémenter les mécanismes d’évolution dynamique modifiant la carte au cours du temps.
* **Livrable :** Code gérant la simulation naturelle du monde.

---

### Phase 4 – Simulation des civilisations

#### T5 : Création de structures (bâtiments)
* **Développeur assigné :** Dev 1
* **Description :** Intégrer un système de création permettant le placement et la gestion d'un ensemble de structures plausibles.
* **Livrable :** Code permettant la gestion des bâtiments sur la carte.

#### T6 : Colonies & croissance urbaine
* **Développeur assigné :** Dev 2
* **Description :** Développer un système permettant la création de colonies et l'expansion territoriale.
* **Livrable :** Villes visibles sur la carte évoluant au cours du temps.

---

### Phase 5 – Interface et finalisation

#### T7 : Interface utilisateur & contrôle sim.
* **Développeurs assignés :** Dev 1 & 2
* **Description :** Permettre à l’utilisateur de générer une carte, modifier les paramètres et contrôler la vitesse de simulation.
* **Livrable :** Interface fonctionnelle.

#### T8 : Tests, corrections & livraison finale
* **Développeurs assignés :** Dev 1 & 2
* **Description :** Vérification globale du code, résolution des bugs finaux et préparation de la soutenance.
* **Livrable :** Version finale du programme prête à être évaluée.