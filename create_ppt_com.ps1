# Créer une présentation PowerPoint via COM
$PowerPoint = New-Object -ComObject PowerPoint.Application
$PowerPoint.Visible = [int]1

# Créer une nouvelle présentation
$Presentation = $PowerPoint.Presentations.Add([int]1)

# Slide 1: Titre (ppLayoutTitleSlide = 6)
$Slide1 = $Presentation.Slides.Add(1, 6)
$Slide1.Shapes.Title.TextFrame.Clear()
$Slide1.Shapes.Title.TextFrame.TextRange.Text = "Cat Clicker"
if ($Slide1.Shapes.Count -ge 2) {
    $Slide1.Shapes.Item(2).TextFrame.Clear()
    $Slide1.Shapes.Item(2).TextFrame.TextRange.Text = "Un jeu de clic addictif"
}

# Slide 2: Concept
$Slide2 = $Presentation.Slides.Add(2, 1)
$Slide2.Shapes.Title.TextFrame.Clear()
$Slide2.Shapes.Title.TextFrame.TextRange.Text = "Concept du Jeu"
if ($Slide2.Shapes.Count -ge 2) {
    $Slide2.Shapes.Item(2).TextFrame.Clear()
    $Slide2.Shapes.Item(2).TextFrame.TextRange.Text = "Cliquez sur le chat pour augmenter votre score"
}

# Slide 3: Caractéristiques
$Slide3 = $Presentation.Slides.Add(3, 1)
$Slide3.Shapes.Title.TextFrame.Clear()
$Slide3.Shapes.Title.TextFrame.TextRange.Text = "Caractéristiques"
if ($Slide3.Shapes.Count -ge 2) {
    $Slide3.Shapes.Item(2).TextFrame.Clear()
    $Slide3.Shapes.Item(2).TextFrame.TextRange.Text = "7 niveaux progressifs avec difficulté croissante`nThèmes de couleurs personnalisés`nEffets sonores immersifs`nSystème de vies avec bonus`nScore persistant"
}

# Slide 4: Mécaniques
$Slide4 = $Presentation.Slides.Add(4, 1)
$Slide4.Shapes.Title.TextFrame.Clear()
$Slide4.Shapes.Title.TextFrame.TextRange.Text = "Mécaniques du Jeu"
if ($Slide4.Shapes.Count -ge 2) {
    $Slide4.Shapes.Item(2).TextFrame.Clear()
    $Slide4.Shapes.Item(2).TextFrame.TextRange.Text = "Clic de souris pour scorer`nProgression automatique entre niveaux`nMultiplicateurs de points`nLimite de temps par niveau"
}

# Slide 5: Technologie
$Slide5 = $Presentation.Slides.Add(5, 1)
$Slide5.Shapes.Title.TextFrame.Clear()
$Slide5.Shapes.Title.TextFrame.TextRange.Text = "Technologie"
if ($Slide5.Shapes.Count -ge 2) {
    $Slide5.Shapes.Item(2).TextFrame.Clear()
    $Slide5.Shapes.Item(2).TextFrame.TextRange.Text = "Développé en C avec SDL2`nGraphismes haute performance`nGestion audio intégrée`nInterface utilisateur personnalisée"
}

# Slide 6: Objectifs de Jeu
$Slide6 = $Presentation.Slides.Add(6, 1)
$Slide6.Shapes.Title.TextFrame.Clear()
$Slide6.Shapes.Title.TextFrame.TextRange.Text = "Objectifs"
if ($Slide6.Shapes.Count -ge 2) {
    $Slide6.Shapes.Item(2).TextFrame.Clear()
    $Slide6.Shapes.Item(2).TextFrame.TextRange.Text = "Atteindre le niveau 7`nMaximiser votre score`nCompléter tous les thèmes`nBattre vos meilleurs scores"
}

# Slide 7: Merci
$Slide7 = $Presentation.Slides.Add(7, 6)
$Slide7.Shapes.Title.TextFrame.Clear()
$Slide7.Shapes.Title.TextFrame.TextRange.Text = "Merci!"
if ($Slide7.Shapes.Count -ge 2) {
    $Slide7.Shapes.Item(2).TextFrame.Clear()
    $Slide7.Shapes.Item(2).TextFrame.TextRange.Text = "Amusez-vous bien avec Cat Clicker!"
}

# Sauvegarder la présentation
$SavePath = "c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.pptx"
$Presentation.SaveAs($SavePath, 1)  # ppSaveAsDefault

Write-Host "✅ Présentation créée avec succès: $SavePath"

# Laisser PowerPoint ouvert pour vérification
Start-Sleep -Seconds 2
