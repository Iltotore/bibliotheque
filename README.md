# Bibliothèque

Il s'agit du projet d'informatique de Pré ING 1 à CYTech, fait par Raphaël FROMENTIN et Rachel Emessiene.

## Lancement

Vous pouvez compiler le projet via la commande `make` et lancer l'exécutable `app.out`. Vous pouvez également utiliser la tâche `make run` qui va directement compiler et exécuter le projet.

**⚠️ Le Makefile ne fonctionne que sous Linux**

**⚠️ Nécessite Make et GCC**

## Utilisation

### Connexion/inscription
Au lancement, il vous sera demandé de vous connecter ou de vous inscrire. Lors de l'inscription, les critères suivant doivent-être respectés pour le mot de passe:
- Au moins 1 minuscule
- Au moins 1 majuscule
- Au moins 1 chiffre
- Avoir un mot de passe entre 8 et 100 caractères

### Emprunt/remise

Chaque utilisateur peut emprunter plusieurs livres et les rendre dans le délai imposé.
- Étudiant: 3 livres pendant 2 minutes
- Professeur ou Administrateur: 5 livres pendant 3 minutes

### Autres fonctionalités

Voici un récapitulatif des différentes fonctionnalités du projet:

| Fonctionnalité            | Étudiant | Professeur | Administrateur |
|---------------------------|----------|------------|----------------|
| Trier les livres          |✅        |✅         |✅              |
| Emprunter un livre        |✅        |✅         |✅              |
| Rendre un livre           |✅        |✅         |✅              |
| Ajouter un livre          |           |✅         |✅             |
| Supprimer un livre        |           |✅         |✅             |
| Bannir un utilisateur     |           |           |✅              |
| Débannir un utilisateur   |           |           |✅              |
| Promouvoir un utilisateur |           |           |✅              |
