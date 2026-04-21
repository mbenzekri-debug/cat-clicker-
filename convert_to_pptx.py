import subprocess
import os

html_path = r"c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.html"
output_path = r"c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.pptx"

# Converter HTML to PPTX using LibreOffice
try:
    cmd = [
        r"C:\Program Files\LibreOffice\program\soffice.exe",
        "--headless",
        "--convert-to", "pptx",
        "--outdir", r"c:\Users\amity\OneDrive\Desktop\cat game",
        html_path
    ]
    
    result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
    print("Conversion done!")
    print(f"Output: {result.stdout}")
    if result.stderr:
        print(f"Error: {result.stderr}")
        
except Exception as e:
    print(f"Error: {e}")
    # Fallback: try alternative path
    try:
        cmd = [
            r"C:\Program Files (x86)\LibreOffice\program\soffice.exe",
            "--headless",
            "--convert-to", "pptx",
            "--outdir", r"c:\Users\amity\OneDrive\Desktop\cat game",
            html_path
        ]
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30)
        print("Conversion done!")
    except:
        print("Could not find LibreOffice")
