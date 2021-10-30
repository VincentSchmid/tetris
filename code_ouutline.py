

MainLoop:

	System currSystem;

	ActiveSystem.run()


class Menu : System
	
	OnStart()

	OnQuit()


Enum GameStates:
	
	IsPlacing
	GameOver
	Placed

class Map:
	width
	height
	arr # bottom left is 0,0

	checkCollision(shape):

	checkLines():

	moveActiveObject(x, y):
		shapeTransform.x + x
		shapeTransform.y + y

	getMapState():
		return arr + activeShape.matrix * shapeTransform

	getFrozenBlocks():
		return arr

	getActiveShape():
		return activeShape * shapeTransform

class Shape:
	width
	height
	positions # bottom left is 0,0
	position
	nextPosition

	getMoveLeft()

	getMoveRight()

	getRotateLeft()

	getRotateRight()

	moveLeft()

	moveRight()

	rotateLeft()

	rotateRight()


Game : System:

	State gameState
	ShapeFactory shapeFactory
	Shape currShape

	MoveShape(newPos, moveFunc):
		if Map.CheckCollision(newPos):
			moveFunc()

	MoveLeft():
		MoveShape(currShape.GetMoveLeft(), currShape.MoveLeft)

	MoveRight():
		MoveShape(currShape.GetMoveRight(), currShape.MoveRight)

	RotateLeft():
		MoveShape(currShape.GetRotateLeft(), currShape.RotateLeft)

	RotateRight():
		MoveShape(currShape.GetRotateRight(), currShape.RotateRight)


	update():
		if state == placed:
			currShape = shapes.getShape()
			shape.position = map.get_entry(shape.width)
			state = IsPlacing

		colliding = map.checkCollision(shape.nextPosition)

		if colliding:
			state = placed

		else
			shape.position = shape.nextPosition

		numLines = map.clearLines()
		draw()
			
Actions :
	
	action(function):
		function()
		draw()

	RotateLeft:
		action(GameCurrShape.getRotateLeft)
	RotateRight:
		action(GameCurrShape.rotateRight)
	MoveLeft:
		action(GameCurrShape.moveLeft)
	MoveRight:
		action(GameCurrShape.moveRight)


Input :
	LeftKey():
		Actions.MoveLeft()
	RightKey():
		Actions.MoveRight()
	UpKey():
		Actions.RotateLeft()
	DownKey():
		Action.RotateLeft()