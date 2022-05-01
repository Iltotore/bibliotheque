.DEFAULT_GOAL:=app

clean:
	@rm -f *.o
	@rm -f app

gui.c: model.h util.c

main.c: model.h gui.c

app: main.c
	@echo "Building application..."
	@gcc main.c -o app

run: app
	@echo "Running application..."
	@./app
