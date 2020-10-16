var headerDiv = null;
var subHeaderDiv = null;

var registeredImages = [];
var allLowRegistered = false;
var highRes = [];

function addHighRes(container, high, low) {
	highRes.push({
		container: container,
		high: high,
		low: low
	});
}

function tickLowRegistered() {
	if (allLowRegistered) {
		//for (i = 0; i < highRes.length; i++) {
		//	let img = highRes[i];
		//	img.container.replaceChild(img.high, img.low);
		//}

		//highRes = [];
	} else {
		let found = false;
		console.log(registeredImages.length);
		for (i = 0; i < registeredImages.length; i++) {
			if (!registeredImages[i].low) {
				found = true;
				break;
			}
		}

		if (!found) {
			console.log("loading big", registeredImages);
			allLowRegistered = true;

			for (i = 0; i < registeredImages.length; i++) {
				console.log("reeeee");
				let img = registeredImages[i];
				let big = document.createElement("img");

				big.onload = function() {
					console.log("re"); 
					img.high = true;
					img.container.replaceChild(big, img.element);
					big.style.display = "block";
				};

				big.style.display = "none";
				big.src = "https://applesthepi.com/res/512/" + img.src + ".png";
				//big.width = img.size.x.toString();
				//big.height = img.size.y.toString();
			}
		}
	}
}

function registerImage(imageContainer, element, src, size) {
	if (size == null) {
		size = {x: 256, y: 256};
	}

	const id = registeredImages.length;

	registeredImages.push({
		low: false,
		high: false,
		src: src,
		container: imageContainer,
		element: element,
		size: size,
	});
	
	element.style.display = "none";
	element.src = "https://applesthepi.com/res/64/" + src + ".png";
	//element.width = size.x.toString();
	//element.height = size.y.toString();

	element.onload = function() {
		let reg = registeredImages[id];
		reg.low = true;
		imageContainer.appendChild(element);
		element.style.display = "block";
	};
}

function updateHeader() {
	const height = headerDiv.offsetHeight;
	subHeaderDiv.style = "height: calc(100% - " + height + "px); top: " + height + "px;";

	tickLowRegistered();
}

function runHeader() {
	headerDiv = document.getElementsByClassName("super_header")[0];
	subHeaderDiv = document.getElementsByClassName("super_container")[0];

	updateHeader();
	setInterval(updateHeader, 50);
}