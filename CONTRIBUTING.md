# Guide de Contribution - Cat Clicker 🐱

Merci de l'intérêt que vous portez à Cat Clicker! Nous accueillons toutes les contributions.

## Comment Contribuer

### 1️⃣ Signaler un Bug

**Avant de signaler:**
- Vérifiez que le bug n'a pas déjà été reporté
- Testez avec la dernière version

**Pour signaler:**
1. Allez à [Issues](https://github.com/maryambenzekri/cat-clicker/issues)
2. Cliquez sur "New Issue"
3. Utilisez le template "Bug Report"
4. Décrivez:
   - **Étapes pour reproduire**
   - **Comportement attendu**
   - **Comportement réel**
   - **Environnement** (OS, compilateur)

### 2️⃣ Proposer une Fonctionnalité

1. Allez à [Issues](https://github.com/maryambenzekri/cat-clicker/issues)
2. Créez une "Feature Request"
3. Décrivez:
   - **Fonctionnalité désirée**
   - **Cas d'usage**
   - **Bénéfices attendus**

### 3️⃣ Soumettre une Pull Request

```bash
# 1. Fork le repository
git clone https://github.com/votre-username/cat-clicker.git
cd cat-clicker

# 2. Créez une branche
git checkout -b feature/ma-fonctionnalite
# ou
git checkout -b fix/mon-bug

# 3. Faites vos changements
# Suivez les conventions de code (voir ci-dessous)

# 4. Testez!
cmake -B build
cd build
cmake --build .
./cat_clicker

# 5. Committez avec messages clairs
git add .
git commit -m "Add/Fix: Description claire et concise"

# 6. Pushez
git push origin feature/ma-fonctionnalite

# 7. Créez une Pull Request sur GitHub
# Décrivez les changements et le why
```

## 📋 Conventions de Code

### Nommage
```c
// Variables
int player_score;
char* game_state;

// Fonctions
void update_game_state();
int calculate_points();

// Constants
#define MAX_LEVELS 7
#define GAME_WIDTH 800
```

### Style
```c
// Indentation: 4 espaces
if (condition) {
    // Code
}

// Espacez les opérateurs
int result = a + b;

// Commentaires explicatifs
// Mauvais: increment i
i++;

// Bon: Move to next level after score reached
move_to_next_level();
```

### En-tête de Fichier
```c
/*
 * File: cat_clicker.c
 * Description: Main game loop and logic
 * Authors: Maryam Benzekri, Chada Hennouk
 * Created: April 2026
 * Last Modified: April 2026
 */
```

## 📝 Checklist pour PR

- [ ] Le code compile sans warnings
- [ ] Les changements respectent les conventions
- [ ] Les commentaires sont clairs
- [ ] Les fonctions sont testées
- [ ] Les messages de commit sont descriptifs
- [ ] Pas de fichiers inutiles committés

## 🎯 Types de Contributions

### 🐛 Bug Fixes
```
prefix: fix/
exemple: fix/click-detection-bug
description: "Fix click detection not working on Linux"
```

### ✨ Nouvelles Fonctionnalités
```
prefix: feature/
exemple: feature/leaderboard
description: "Add global leaderboard system"
```

### 📚 Documentation
```
prefix: docs/
exemple: docs/api-reference
description: "Add comprehensive API documentation"
```

### 🎨 Amélioration de Code
```
prefix: refactor/
exemple: refactor/event-handling
description: "Refactor event system for better maintainability"
```

### ⚡ Performance
```
prefix: perf/
exemple: perf/rendering-optimization
description: "Optimize rendering pipeline for 120 FPS"
```

## 💬 Questions?

- 📧 Email: contact@catclicker.dev
- 💬 Discussions: GitHub Discussions
- 🐙 GitHub Issues: Pour les questions techniques

## 🙏 Remerciements

Chaque contribution, grande ou petite, aide à rendre Cat Clicker meilleur!

---

**Happy Clicking!** 🐱✨
