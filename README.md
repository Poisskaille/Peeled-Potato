Projet Flappy Bird
Ce projet consiste en une réinterprétation 3D du classique Flappy Bird, développée en C++ sur une période de trois semaines. Le jeu s'appuie sur Termina, un moteur de jeu custom.

### 👥 Développeurs
Kiliann Pommez

Adrien Moreau

### 🛠️ Stack Technique
Langage : C++17 / C++20

Moteur : Termina Engine (Custom)

Graphismes : Vulkan 

### 🏗️ Architecture du Moteur "Termina"
Le moteur a été conçu pour répondre aux contraintes d'un développement rapide (3 semaines) tout en gérant une perspective 3D.

Points clés de l'implémentation :
Gestionnaire de Scène : Système de cycle de vie des objets (Update/Draw).

Pipeline de Rendu : Gestion des transformations 3D (Matrices Model-View-Projection).

Gestion des Entrées : Abstraction des inputs clavier/souris pour le contrôle du "saut".

### 🎮 Le Jeu : Flappy Bird 3D
L'objectif était de transposer le gameplay 2D addictif dans un espace tridimensionnel tout en maintenant une lisibilité de l'action.

## Input :
- barre espace pour sauter

### 🚀 Installation et Compilation
## Prérequis :

Compilateur supportant le C++17 (GCC, Clang, ou MSVC).
Instalation de xmake pour pouvoir lancer le projet
dans le terminal entrer "xmake" puis "xmake run Editor".

Une fois le moteur lancer aller dans World > Open > Assets > Worlds > CustomScene > Sceneplayer

Puis lancer le jeu en cliquant sur play
