var headerDiv = null;
var subHeaderDiv = null;

function updateHeader() {
	const height = headerDiv.offsetHeight;
	subHeaderDiv.style = "height: calc(100% - " + height + "px); top: " + height + "px;";
}

function runHeader() {
	headerDiv = document.getElementsByClassName("super_header")[0];
	subHeaderDiv = document.getElementsByClassName("super_container")[0];

	setInterval(updateHeader, 50);
}