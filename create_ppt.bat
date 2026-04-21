@echo off
cd /d "c:\Users\amity\OneDrive\Desktop\cat game"

powershell -ExecutionPolicy Bypass -NoProfile -Command ^
    "$pptx = 'Cat_Clicker_Presentation.pptx'; " ^
    "$ppt = New-Object -Com PowerPoint.Application; " ^
    "$prs = $ppt.Presentations.Add(); " ^
    "$s = $prs.Slides.Add(1,6); $s.Shapes.Title.TextFrame.TextRange.Text = 'Cat Clicker'; " ^
    "$s = $prs.Slides.Add(2,1); $s.Shapes.Title.TextFrame.TextRange.Text = 'Concept'; $s.Shapes.Item(2).TextFrame.TextRange.Text = 'Cliquez sur le chat'; " ^
    "$s = $prs.Slides.Add(3,1); $s.Shapes.Title.TextFrame.TextRange.Text = 'Caracteristiques'; $s.Shapes.Item(2).TextFrame.TextRange.Text = '7 niveaux`n2 themes`nEffets sonores`nVies: 3`nScore persistant'; " ^
    "$s = $prs.Slides.Add(4,1); $s.Shapes.Title.TextFrame.TextRange.Text = 'Mecaniques'; $s.Shapes.Item(2).TextFrame.TextRange.Text = 'Clic souris`nProgression`nMultiplicateurs`nLimite temps'; " ^
    "$s = $prs.Slides.Add(5,1); $s.Shapes.Title.TextFrame.TextRange.Text = 'Technologie'; $s.Shapes.Item(2).TextFrame.TextRange.Text = 'C SDL2`nGraphismes haute perf`nAudio integree`nInterface personnalisee'; " ^
    "$s = $prs.Slides.Add(6,1); $s.Shapes.Title.TextFrame.TextRange.Text = 'Objectifs'; $s.Shapes.Item(2).TextFrame.TextRange.Text = 'Niveau 7`nScore max`nTous themes`nMeilleurs scores'; " ^
    "$s = $prs.Slides.Add(7,6); $s.Shapes.Title.TextFrame.TextRange.Text = 'Merci!'; " ^
    "$prs.SaveAs((Resolve-Path '.').Path + '\' + $pptx, 1); " ^
    "Write-Host 'DONE'"

pause
