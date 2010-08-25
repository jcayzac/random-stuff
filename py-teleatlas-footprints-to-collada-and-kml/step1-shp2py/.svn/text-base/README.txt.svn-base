SHP2PY

Script de conversion des données TeleAtlas "Citymaps 3D" vers python.

Ce script:
	- Utilise la commande d'import Oracle "shp2sdo.exe" pour convertir les fichiers ShapeFile en SQL, plus simple à interpréter
	- Utilise Blender (version 2.45) comme interpréteur Python, et profite de ses fonctions géométriques pour générer des volumes en trianglestrips à partir des polygones 2D issus des données TeleAtlas, et calculer automatiquement les normales.
	- Exporte le résultat au format Python (équivalent JSON)

Usage:
	shp2py.cmd input_path output_path

Notes:
	- shp2sdo.exe doit être dans le PATH lorsque le script est invoqué (c'est déjà le cas si le script est invoqué depuis son répertoire d'origine)
	- Le script crée un fichier "bldresumeat.bin" dans le répertoire d'exécution, permettant de le couper et de reprendre là où il s'était arréter (Blender leake un peu de mémoire et ça peut-être nécessaire au bout d'un moment). Si on veut recommencer à zéro, ou travailler sur une autre base, il faut effacer ce fichier.


