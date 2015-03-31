var image = new Image();
var canvas = document.getElementById("stage27");
var context = canvas.getContext("2d");
var WIDTH = canvas.width;
var HEIGHT = canvas.height;
var inputSize = 20;
var stick = new Stick(inputSize);
var limitSize = 88;
var previousX = 0;
var previousY = 0;

$(document).ready(init());


function init() {
	console.log("IN INIT");
    centerStick();
    
    canvas.addEventListener("touchstart", function(e) {
        e.preventDefault();
        var touch = e.touches[0];
        stick.setInputXY(touch.pageX, touch.pageY);
        stick.active = true;
    });

	canvas.addEventListener("touchmove", function (e) {
		e.preventDefault();

		var touch = e.touches[0];
		stick.setInputXY(touch.pageX, touch.pageY);
    });        

	canvas.addEventListener("touchend", function (e) {
        stick.active = false;
	});        
        
    image.src = "images/stick.png";
    image.onload = function() {
      setInterval(main, 1);  
    };
}
    
function main() {
    update();
    draw();
}

function draw() {
    context.clearRect(0, 0, WIDTH, HEIGHT);
    drawStick();
}

function drawStick() {
	context.save();

	// Limit
	context.drawImage(
		image,
		0, 0,
		88, 88,
		stick.limit.x - (limitSize / 2), stick.limit.y - (limitSize / 2),
		limitSize, limitSize
	);

	// Input
	var knobSize = 60;
	context.drawImage(
		image,
		89, 14,
		knobSize, knobSize,
		stick.input.x - (knobSize / 2), stick.input.y - (knobSize / 2),
		knobSize, knobSize
	);

	context.restore();        
}

function update() {
    stick.update();
    
    var xvalue = Math.round((stick.normal.x + 1) * 90);
    var yvalue = Math.round((stick.normal.y + 1) * 90);
	if (xvalue != previousX || yvalue != previousY) {
		console.log(xvalue + "x" + yvalue);
		previousX = xvalue;
		previousY = yvalue;
		SendCommand("27/20/" + xvalue + "/" + yvalue);
	}
}

function centerStick() {
    stick.setLimitXY(WIDTH / 2, HEIGHT / 2);
    stick.setInputXY(WIDTH / 2, HEIGHT / 2);
}

function setStick(x, y) {
    var newX = 0;
    var newY = 0;
    
    if (x == 90)
        newX = WIDTH /2;
    else if (x==180)
        newX = WIDTH;
    
    if (y == 90)
        newY = HEIGHT /2;
    else if (y==180)
        newY = HEIGHT;

    stick.setInputXY(newX, newY);
}
    

