# Cat Clicker (SDL2, C)

Jeu 2D en C pur avec SDL2.

## Fonctionnalites

- Fenetre SDL2 800x600
- **Menu de sélection de thème au démarrage** (Dark Mode / Light Mode)
- **Système de niveaux progressifs** 📈
  - Chaque niveau demande 5 chats de plus à cliquer
  - Les ennemis apparaissent plus vite à chaque niveau
  - Les chats se déplacent plus rapidement
  - Max 10 niveaux (30, 35, 40... 80 chats pour le niveau 10)
- Chats dans un tableau de taille max 20
- Spawn aleatoire avec probabilites:
  - 70% chats colores
  - 30% chats noirs
- Clic souris:
  - chat colore: score +1 (son "meow") → avance le compteur de niveau
  - chat noir: vie -1 (son "hit")
- 3 vies au depart
- Game Over a 0 vie (son grave)
- Difficulte progressive:
  - spawn de plus en plus rapide
  - vitesse de deplacement augmente
  - Augmente davantage à chaque nouveau niveau
- Redemarrage apres Game Over:
  - touche `R`
  - ou clic sur le bouton `RESTART`
- **Themes personnalisables (Dark/Light)**:
  - Dark Mode: Nuit étoilée avec lune
  - Light Mode: Jour ensoleillé avec nuages
- **Audio genere proceduralement** (beeps dynamiques)
- Nettoyage SDL + SDL_mixer propre en fin d'execution

## Fichier principal

- `cat_clicker.c`

## Build (Windows / MSYS2 UCRT64)

### 1) Installer SDL2 et SDL2_mixer (une seule fois)

```powershell
C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm --needed mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_mixer"
```

### 2) Compiler

```powershell
gcc cat_clicker.c -o cat_clicker.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lm
```

### 3) Lancer

```powershell
.\cat_clicker.exe
```

## Commandes en jeu

### Selection du theme (au demarrage)
- **Clic** sur "DARK MODE" ou "LIGHT MODE"
- **Touche D**: Selectionner Dark Mode
- **Touche L**: Selectionner Light Mode

### Pendant le jeu
- **Clic gauche**: cliquer sur les chats colorés pour augmenter le score
- Atteindre le nombre de chats requis pour passer au niveau suivant
- **T**: basculer entre Dark Mode et Light Mode
- **R**: restart quand Game Over
- **Esc**: quitter

## Système de Niveau

- **Niveau 1**: 5 chats à cliquer
- **Niveau 2**: 10 chats (total)
- **Niveau 3**: 15 chats (total)
- ... et ainsi de suite jusqu'à **Niveau 10: 50 chats (total)**

Chaque niveau:
- Réduit l'intervalle de spawn de 50ms
- Augmente la vitesse des chats de +0.15x
- Joue un son d'ascension (level up)

## Audio

Cinq effectifs sonores generes dynamiquement:
- **Meow** (250-450 Hz, 300 ms): Clic sur chat coloré
- **Hit/Boom** (80-120 Hz, 1200 ms): Clic sur chat noir
- **Game Over** (Descending tones, 2000 ms): Fin de partie
- **Level Up** (Ascending tones, 800 ms): Passage au prochain niveau

Les beeps sont generes en temps reel via SDL_mixer sans fichier externe.

## Themes

### Dark Mode (par defaut)
- Background: Bleu marine fonce
- Texte: Blanc
- Accents: Orange (255, 182, 72)
- Ambiance: Nuit étoilée avec lune

### Light Mode
- Background: Blanc casse leger
- Texte: Gris bleu fonce
- Accents: Orange (255, 140, 50)
- Ambiance: Jour ensoleille avec soleil et nuages
