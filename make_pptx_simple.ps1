$pptxPath = "c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.pptx"

$PowerPoint = New-Object -ComObject PowerPoint.Application

# Créer une présentation
$Presentation = $PowerPoint.Presentations.Add()

# Slide 1: Titre
$Slide1 = $Presentation.Slides.Add(1, 6)
$Slide1.Shapes.Title.TextFrame.TextRange.Text = "Cat Clicker"

# Slide 2
$Slide2 = $Presentation.Slides.Add(2, 1)
$Slide2.Shapes.Title.TextFrame.TextRange.Text = "Concept du Jeu"
$Slide2.Shapes.Item(2).TextFrame.TextRange.Text = "Cliquez sur le chat pour augmenter votre score"

# Slide 3
$Slide3 = $Presentation.Slides.Add(3, 1)
$Slide3.Shapes.Title.TextFrame.TextRange.Text = "Caractéristiques"
$Slide3.Shapes.Item(2).TextFrame.TextRange.Text = "7 niveaux progressifs`n2 thèmes de couleurs`nEffets sonores`nSystème de vies`nScore persistant"

# Slide 4
$Slide4 = $Presentation.Slides.Add(4, 1)
$Slide4.Shapes.Title.TextFrame.TextRange.Text = "Mécaniques"
$Slide4.Shapes.Item(2).TextFrame.TextRange.Text = "Clic de souris`nProgression automatique`nMultiplicateurs de points`nLimite de temps"

# Slide 5
$Slide5 = $Presentation.Slides.Add(5, 1)
$Slide5.Shapes.Title.TextFrame.TextRange.Text = "Technologie"
$Slide5.Shapes.Item(2).TextFrame.TextRange.Text = "Développé en C avec SDL2`nGraphismes haute performance`nGestion audio intégrée"

# Slide 6
$Slide6 = $Presentation.Slides.Add(6, 1)
$Slide6.Shapes.Title.TextFrame.TextRange.Text = "Objectifs"
$Slide6.Shapes.Item(2).TextFrame.TextRange.Text = "Atteindre le niveau 7`nMaximiser votre score`nCompléter tous les thèmes"

# Slide 7: Merci
$Slide7 = $Presentation.Slides.Add(7, 6)
$Slide7.Shapes.Title.TextFrame.TextRange.Text = "Merci!"

# Sauvegarder (format ppSaveAsDefault = 1)
$Presentation.SaveAs($pptxPath, 1)

Write-Host "Présentation créée: $pptxPath"

# Attendre un peu pour la finalisation
Start-Sleep -Seconds 1

# Fermer mais garder PowerPoint ouvert pour vérification
