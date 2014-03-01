
.PHONY: tetris1.c tetris2.c tetris3.c clean

tetris.c: tetris1.c tetris2.c tetris3.c
	cat tetris1.c tetris2.c tetris3.c > tetris.c

tetris1.c:
	python sampler.py < tetris1.csv > tetris1.c

tetris2.c:
	python sampler.py < tetris2.csv > tetris2.c

tetris3.c:
	python sampler.py < tetris3.csv > tetris3.c

clean:
	rm *.c
	
