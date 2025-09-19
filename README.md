Bonjour, bienvenue sur le projet CardVape du WorkShop 2025 de l'epsi.

Les détenteurs du projets sont:
Thibault Royer,
Noa Rodriguez,
Vianney Haag.

Le projet permet actuellement de récupérer les UID de cartes RFID. Des modifications futures permetteront le copiages des cartes.

**Mentions légales:**

Le dispositif « Card-Vape » a été conçu dans un cadre strictement pédagogique et expérimental.
 Il ne doit être utilisé que pour :
des travaux pratiques, des expérimentations personnelles à domicile, un usage purement privé et non commercial.

Toute utilisation du dispositif en dehors de ce cadre (par exemple pour accéder à des zones restreintes, modifier des cartes/badges de machines, obtenir un avantage indu, ou tout autre usage frauduleux) est strictement interdite et peut constituer une infraction pénale (telle que fraude, falsification de moyens de paiement, ou intrusion dans un système).
La reproduction des maquettes, plans et codes fournis est autorisée uniquement pour un usage personnel, éducatif ou de curiosité technique. Toute utilisation à des fins nuisibles ou lucratives est interdite.

Pour utiliser le projet il suffit d'upload le .ino sur l'Arduino MKR1000, connecté en MQTT où le code python de sauvegarde tourne.

La spécification du MQTT est le dossier .conf

L'extension à faire tourner en python (fonctionnel sur l'OS de base de raspberry Pi) est le fichier .py.

Le fichier Json d'enregistrement est directement créé dans le répertoire du code python.


Sources du code:
Merci au code de démonstration d'arduino pour les paramétrages de bases 
