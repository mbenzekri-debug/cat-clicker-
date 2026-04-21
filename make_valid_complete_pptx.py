#!/usr/bin/env python3
import os
import zipfile
from xml.dom.minidom import Document

def add_namespace(tag, ns):
    """Ajoute le namespace au tag."""
    return f"{{{ns}}}{tag}" if ns else tag

def create_complete_pptx():
    output_path = r"c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.pptx"
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    
    with zipfile.ZipFile(output_path, 'w', zipfile.ZIP_DEFLATED) as pptx:
        # [Content_Types].xml - CORRECT
        content_types = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Types xmlns="http://schemas.openxmlformats.org/package/2006/content-types">
<Default Extension="rels" ContentType="application/vnd.openxmlformats-package.relationships+xml"/>
<Default Extension="xml" ContentType="application/xml"/>
<Override PartName="/ppt/presentation.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.presentation.main+xml"/>
<Override PartName="/ppt/slides/slide1.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide2.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide3.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide4.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide5.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide6.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slides/slide7.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slide+xml"/>
<Override PartName="/ppt/slideLayouts/slideLayout1.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml"/>
<Override PartName="/ppt/slideLayouts/slideLayout2.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slideLayout+xml"/>
<Override PartName="/ppt/slideMasters/slideMaster1.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.slideMaster+xml"/>
<Override PartName="/ppt/theme/theme1.xml" ContentType="application/vnd.openxmlformats-officedocument.theme+xml"/>
<Override PartName="/ppt/notesMasters/notesMaster1.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.notesMaster+xml"/>
<Override PartName="/ppt/handoutMasters/handoutMaster1.xml" ContentType="application/vnd.openxmlformats-officedocument.presentationml.handoutMaster+xml"/>
<Override PartName="/docProps/core.xml" ContentType="application/vnd.openxmlformats-package.core-properties+xml"/>
<Override PartName="/docProps/app.xml" ContentType="application/vnd.openxmlformats-officedocument.custom-properties+xml"/>
</Types>'''
        pptx.writestr('[Content_Types].xml', content_types)
        
        # _rels/.rels
        rels = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument" Target="ppt/presentation.xml"/>
<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties" Target="docProps/core.xml"/>
<Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/custom-properties" Target="docProps/app.xml"/>
</Relationships>'''
        pptx.writestr('_rels/.rels', rels)
        
        # docProps/core.xml
        core = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<cp:coreProperties xmlns:cp="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/" xmlns:dcmitype="http://purl.org/dc/dcmitype/" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
<dc:creator>User</dc:creator>
<dc:title>Cat Clicker Presentation</dc:title>
<dcterms:created xsi:type="dcterms:W3CDTF">2026-04-21T00:00:00Z</dcterms:created>
</cp:coreProperties>'''
        pptx.writestr('docProps/core.xml', core)
        
        # docProps/app.xml
        app = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Properties xmlns="http://schemas.openxmlformats.org/officeDocument/2006/custom-properties" xmlns:vt="http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes">
<TotalTime>0</TotalTime>
<Words>0</Words>
<Application>Microsoft Office PowerPoint</Application>
<PresentationFormat>On-screen Show</PresentationFormat>
<Paragraphs>0</Paragraphs>
<Slides>7</Slides>
<Notes>0</Notes>
<HiddenSlides>0</HiddenSlides>
<MMClips>0</MMClips>
<ScaleCrop>false</ScaleCrop>
<HeadingPairs>
<vt:vector baseType="variant" size="12">
<vt:variant><vt:lpstr>Slide Titles</vt:lpstr></vt:variant>
<vt:variant><vt:i4>7</vt:i4></vt:variant>
<vt:variant><vt:lpstr>Numerical Data</vt:lpstr></vt:variant>
<vt:variant><vt:i4>0</vt:i4></vt:variant>
<vt:variant><vt:lpstr>Published</vt:lpstr></vt:variant>
<vt:variant><vt:bool>false</vt:bool></vt:variant>
</vt:vector>
</HeadingPairs>
<TitlesOfParts>
<vt:vector baseType="lpstr" size="7">
<vt:lpstr>Cat Clicker</vt:lpstr>
<vt:lpstr>Concept du Jeu</vt:lpstr>
<vt:lpstr>Caracteristiques</vt:lpstr>
<vt:lpstr>Mecaniques</vt:lpstr>
<vt:lpstr>Technologie</vt:lpstr>
<vt:lpstr>Objectifs</vt:lpstr>
<vt:lpstr>Merci</vt:lpstr>
</vt:vector>
</TitlesOfParts>
</Properties>'''
        pptx.writestr('docProps/app.xml', app)
        
        # ppt/_rels/presentation.xml.rels
        pres_rels = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster" Target="slideMasters/slideMaster1.xml"/>
<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/notesMaster" Target="notesMasters/notesMaster1.xml"/>
<Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/handoutMaster" Target="handoutMasters/handoutMaster1.xml"/>
<Relationship Id="rId4" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide1.xml"/>
<Relationship Id="rId5" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide2.xml"/>
<Relationship Id="rId6" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide3.xml"/>
<Relationship Id="rId7" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide4.xml"/>
<Relationship Id="rId8" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide5.xml"/>
<Relationship Id="rId9" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide6.xml"/>
<Relationship Id="rId10" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slide" Target="slides/slide7.xml"/>
<Relationship Id="rId11" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme" Target="theme/theme1.xml"/>
</Relationships>'''
        pptx.writestr('ppt/_rels/presentation.xml.rels', pres_rels)
        
        # ppt/presentation.xml
        pres = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:presentation xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:rel="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
<p:sldSz cx="9144000" cy="6858000"/>
<p:notesSz cx="10058400" cy="7772400"/>
<p:sldIdLst>
<p:sldId id="256" r:id="rId4"/>
<p:sldId id="257" r:id="rId5"/>
<p:sldId id="258" r:id="rId6"/>
<p:sldId id="259" r:id="rId7"/>
<p:sldId id="260" r:id="rId8"/>
<p:sldId id="261" r:id="rId9"/>
<p:sldId id="262" r:id="rId10"/>
</p:sldIdLst>
<p:sldMasterIdLst>
<p:sldMasterId r:id="rId1"/>
</p:sldMasterIdLst>
<p:notesMasterIdLst>
<p:notesMasterId r:id="rId2"/>
</p:notesMasterIdLst>
<p:handoutMasterIdLst>
<p:handoutMasterId r:id="rId3"/>
</p:handoutMasterIdLst>
<p:penLst/>
</p:presentation>'''
        pptx.writestr('ppt/presentation.xml', pres)
        
        # Créer les slides avec contenu
        slides_data = [
            ("Cat Clicker", "Un jeu de clic addictif"),
            ("Concept du Jeu", "Cliquez sur le chat pour augmenter votre score"),
            ("Caracteristiques", "7 niveaux progressifs, 2 themes, Effets sonores, Systeme de vies, Score persistant"),
            ("Mecaniques", "Clic de souris, Progression automatique, Multiplicateurs de points, Limite de temps"),
            ("Technologie", "Developpe en C avec SDL2, Graphismes haute performance, Gestion audio integree"),
            ("Objectifs", "Atteindre le niveau 7, Maximiser votre score, Completer tous les themes"),
            ("Merci", "Amusez-vous bien avec Cat Clicker"),
        ]
        
        for i, (title, content) in enumerate(slides_data, 1):
            slide_xml = f'''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:sld xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
<p:cSld>
<p:bg><p:bgPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="9144000" cy="6858000"/></a:xfrm><a:prstFill><a:solidFill><a:srgbClr val="FFFFFF"/></a:solidFill></a:prstFill><a:effectLst/></p:bgPr></p:bg>
<p:spTree>
<p:nvGrpSpPr>
<p:cNvPr id="1" name="Title 1"/>
<p:cNvGrpSpPr/>
<p:nvPr/>
</p:nvGrpSpPr>
<p:grpSpPr>
<a:xfrm>
<a:off x="0" y="0"/>
<a:ext cx="9144000" cy="6858000"/>
<a:chOff x="0" y="0"/>
<a:chExt cx="9144000" cy="6858000"/>
</a:xfrm>
<a:prstGeom prst="rect"><a:avLst/></a:prstGeom>
</p:grpSpPr>
<p:sp>
<p:nvSpPr>
<p:cNvPr id="2" name="Title 1"/>
<p:cNvSpPr><p:spLocks noGrp="1"/></p:cNvSpPr>
<p:nvPr/>
</p:nvSpPr>
<p:spPr>
<a:xfrm><a:off x="457200" y="274638"/><a:ext cx="8229600" cy="1143000"/></a:xfrm>
<a:prstGeom prst="rect"><a:avLst/></a:prstGeom>
<a:noFill/>
</p:spPr>
<p:txBody>
<a:bodyPr rtlCol="0" anchor="ctr"/>
<a:lstStyle/>
<a:p><a:pPr algn="ctr"/><a:r><a:rPr lang="en-US" sz="4400" bold="1"/><a:t>{title}</a:t></a:r></a:p>
</p:txBody>
</p:sp>
<p:sp>
<p:nvSpPr>
<p:cNvPr id="3" name="Content 2"/>
<p:cNvSpPr><p:spLocks noGrp="1"/></p:cNvSpPr>
<p:nvPr/>
</p:nvSpPr>
<p:spPr>
<a:xfrm><a:off x="457200" y="1417638"/><a:ext cx="8229600" cy="5016000"/></a:xfrm>
<a:prstGeom prst="rect"><a:avLst/></a:prstGeom>
<a:noFill/>
</p:spPr>
<p:txBody>
<a:bodyPr rtlCol="0"/>
<a:lstStyle/>
<a:p><a:pPr><a:buNone/></a:pPr><a:r><a:rPr lang="en-US" sz="2800"/><a:t>{content}</a:t></a:r></a:p>
</p:txBody>
</p:sp>
</p:spTree>
</p:cSld>
<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>
</p:sld>'''
            pptx.writestr(f'ppt/slides/slide{i}.xml', slide_xml)
        
        # Créer slideLayouts
        for layout_num in [1, 2]:
            layout_xml = f'''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:sldLayout xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
<p:cSld>
<p:spTree>
<p:nvGrpSpPr>
<p:cNvPr id="1" name="Title Slide Layout {layout_num}"/>
<p:cNvGrpSpPr/>
<p:nvPr><p:ph/></p:nvPr>
</p:nvGrpSpPr>
<p:grpSpPr>
<a:xfrm><a:off x="0" y="0"/><a:ext cx="9144000" cy="6858000"/><a:chOff x="0" y="0"/><a:chExt cx="9144000" cy="6858000"/></a:xfrm>
<a:prstGeom prst="rect"><a:avLst/></a:prstGeom>
</p:grpSpPr>
</p:spTree>
</p:cSld>
<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>
</p:sldLayout>'''
            pptx.writestr(f'ppt/slideLayouts/slideLayout{layout_num}.xml', layout_xml)
        
        # Créer slideMaster
        master_xml = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:sldMaster xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
<p:cSld>
<p:bg><p:bgPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="9144000" cy="6858000"/></a:xfrm><a:prstFill><a:solidFill><a:srgbClr val="FFFFFF"/></a:solidFill></a:prstFill><a:effectLst/></p:bgPr></p:bg>
<p:spTree>
<p:nvGrpSpPr>
<p:cNvPr id="1" name="Slide Master"/>
<p:cNvGrpSpPr/>
<p:nvPr/>
</p:nvGrpSpPr>
<p:grpSpPr>
<a:xfrm><a:off x="0" y="0"/><a:ext cx="9144000" cy="6858000"/><a:chOff x="0" y="0"/><a:chExt cx="9144000" cy="6858000"/></a:xfrm>
<a:prstGeom prst="rect"><a:avLst/></a:prstGeom>
</p:grpSpPr>
</p:spTree>
</p:cSld>
<p:clrMapOvr><a:masterClrMapping/></p:clrMapOvr>
<p:sldLayoutIdLst>
<p:sldLayoutId id="2147483649" r:id="rId1"/>
<p:sldLayoutId id="2147483650" r:id="rId2"/>
</p:sldLayoutIdLst>
</p:sldMaster>'''
        pptx.writestr('ppt/slideMasters/slideMaster1.xml', master_xml)
        
        # Créer slideLayouts rels
        for layout_num in [1, 2]:
            layout_rels = f'''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideMaster" Target="../slideMasters/slideMaster1.xml"/>
<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme" Target="../theme/theme1.xml"/>
</Relationships>'''
            pptx.writestr(f'ppt/slideLayouts/_rels/slideLayout{layout_num}.xml.rels', layout_rels)
        
        # Créer slideMaster rels
        master_rels = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<Relationships xmlns="http://schemas.openxmlformats.org/package/2006/relationships">
<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout" Target="../slideLayouts/slideLayout1.xml"/>
<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/slideLayout" Target="../slideLayouts/slideLayout2.xml"/>
<Relationship Id="rId3" Type="http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme" Target="../theme/theme1.xml"/>
</Relationships>'''
        pptx.writestr('ppt/slideMasters/_rels/slideMaster1.xml.rels', master_rels)
        
        # Créer theme
        theme_xml = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<a:theme xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" name="Office Theme">
<a:themeElements>
<a:clrScheme name="Office">
<a:dk1><a:srgbClr val="000000"/></a:dk1>
<a:lt1><a:srgbClr val="FFFFFF"/></a:lt1>
<a:dk2><a:srgbClr val="1F497D"/></a:dk2>
<a:lt2><a:srgbClr val="EEECE1"/></a:lt2>
<a:accent1><a:srgbClr val="4472C4"/></a:accent1>
<a:accent2><a:srgbClr val="ED7D31"/></a:accent2>
<a:accent3><a:srgbClr val="A5A5A5"/></a:accent3>
<a:accent4><a:srgbClr val="FFC000"/></a:accent4>
<a:accent5><a:srgbClr val="5B9BD5"/></a:accent5>
<a:accent6><a:srgbClr val="70AD47"/></a:accent6>
<a:hyperlink><a:srgbClr val="0563C1"/></a:hyperlink>
<a:folHyperlink><a:srgbClr val="954F72"/></a:folHyperlink>
</a:clrScheme>
<a:fontScheme name="Office">
<a:majorFont><a:latin typeface="Calibri" panose="020B0604020202020204"/><a:ea typeface=""/><a:cs typeface=""/></a:majorFont>
<a:minorFont><a:latin typeface="Calibri" panose="020B0604020202020204"/><a:ea typeface=""/><a:cs typeface=""/></a:minorFont>
</a:fontScheme>
<a:fmtScheme name="Office">
<a:fillStyleLst>
<a:solidFill><a:schemeClr val="phClr"/></a:solidFill>
<a:gradFill rotWithShape="1"><a:gsLst><a:gs pos="0"><a:schemeClr val="phClr"><a:lumMod val="110000"/><a:satMod val="105000"/><a:shade val="103000"/></a:schemeClr></a:gs><a:gs pos="100000"><a:schemeClr val="phClr"><a:lumMod val="105000"/><a:satMod val="103000"/><a:shade val="106000"/></a:schemeClr></a:gs></a:gsLst><a:lin ang="5400000" scaled="0"/></a:gradFill>
<a:patternFill prst="ltDnDiag"><a:fgClr><a:schemeClr val="phClr"/></a:fgClr><a:bgClr><a:schemeClr val="phClr"><a:lumMod val="105000"/><a:satMod val="103000"/><a:shade val="106000"/></a:schemeClr></a:bgClr></a:patternFill>
</a:fillStyleLst>
<a:lnStyleLst>
<a:ln w="6350"><a:solidFill><a:schemeClr val="phClr"/></a:solidFill><a:prstDash val="solid"/><a:prstCap val="flat"/><a:round/></a:ln>
<a:ln w="12700"><a:solidFill><a:schemeClr val="phClr"/></a:solidFill><a:prstDash val="solid"/><a:prstCap val="flat"/><a:round/></a:ln>
<a:ln w="19050"><a:solidFill><a:schemeClr val="phClr"/></a:solidFill><a:prstDash val="solid"/><a:prstCap val="flat"/><a:round/></a:ln>
</a:lnStyleLst>
<a:effectStyleLst>
<a:effectLst/>
<a:effectLst/>
<a:effectLst/>
</a:effectStyleLst>
<a:bgFillStyleLst>
<a:solidFill><a:schemeClr val="phClr"/></a:solidFill>
<a:solidFill><a:schemeClr val="phClr"><a:lumMod val="105000"/><a:satMod val="103000"/><a:shade val="106000"/></a:schemeClr></a:solidFill>
<a:solidFill><a:schemeClr val="phClr"><a:lumMod val="105000"/><a:satMod val="103000"/><a:tint val="103000"/></a:schemeClr></a:solidFill>
</a:bgFillStyleLst>
</a:fmtScheme>
</a:themeElements>
<a:objectDefaults/>
<a:extraClrSchemeLst/>
</a:theme>'''
        pptx.writestr('ppt/theme/theme1.xml', theme_xml)
        
        # Créer notesMaster
        notes_master = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:notesMaster xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
<p:cSld>
<p:spTree>
<p:nvGrpSpPr><p:cNvPr id="1" name="Notes Master"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr>
<p:grpSpPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="10058400" cy="7772400"/><a:chOff x="0" y="0"/><a:chExt cx="10058400" cy="7772400"/></a:xfrm><a:prstGeom prst="rect"><a:avLst/></a:prstGeom></p:grpSpPr>
</p:spTree>
</p:cSld>
</p:notesMaster>'''
        pptx.writestr('ppt/notesMasters/notesMaster1.xml', notes_master)
        
        # Créer handoutMaster
        handout_master = '''<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<p:handoutMaster xmlns:p="http://schemas.openxmlformats.org/presentationml/2006/main" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">
<p:cSld>
<p:spTree>
<p:nvGrpSpPr><p:cNvPr id="1" name="Handout Master"/><p:cNvGrpSpPr/><p:nvPr/></p:nvGrpSpPr>
<p:grpSpPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="10058400" cy="7772400"/><a:chOff x="0" y="0"/><a:chExt cx="10058400" cy="7772400"/></a:xfrm><a:prstGeom prst="rect"><a:avLst/></a:prstGeom></p:grpSpPr>
</p:spTree>
</p:cSld>
</p:handoutMaster>'''
        pptx.writestr('ppt/handoutMasters/handoutMaster1.xml', handout_master)
    
    print(f"✅ PPTX valide créé: {output_path}")

if __name__ == '__main__':
    create_complete_pptx()
