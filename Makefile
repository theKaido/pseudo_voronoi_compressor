# Makefile
# Auteur : Fares BELHADJ
# Email  : amsi@ai.univ-paris8.fr

# Modification : MATHANARUBAN Jonny
# Email  : jonnymth97@gmail.com

#
# Université Paris VIII

SHELL = /bin/sh
# Définition des commandes utilisées
CC = gcc
MACHINE = $(shell uname -s)
# Déclaration des options du compilateur
# PG_FLAGS = -DOPENGL_1_5


ifeq ($(MACHINE), Darwin)
    # macOS-specific flags and libraries
    GL_LDFLAGS = -framework OpenGL -framework GLUT
else
    # Linux flags and libraries
    GL_LDFLAGS = -lGL -lglut -lGLU
endif

CFLAGS = -Wall
LDFLAGS = -lm $(GL_LDFLAGS)
# Définition des fichiers et dossiers
PROGNAME = compression
HEADERS = ima.h
SOURCES = main.c ppm.c modif.c
# SOURCES = main.c ppm.c modif.c rgb_hls.c
OBJ = $(SOURCES:.c=.o)

all: $(PROGNAME)

$(PROGNAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(PROGNAME)

%.o: %.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	@$(RM) $(PROGNAME) $(OBJ) *~ gmon.out core.*
