fn main() {
	let f_header_body = std::fs::read_to_string("header_body.html").unwrap();
	let f_header_style = std::fs::read_to_string("header_style.css").unwrap();
	let f_header_js = std::fs::read_to_string("header_js.js").unwrap();
	
	for i in std::fs::read_dir("html").unwrap() {
		let file_name = i.unwrap().file_name();
		let file_name = file_name.to_str().unwrap();

		let f_html = std::fs::read_to_string(format!("html/{}", file_name)).unwrap();

		let f_html = f_html.replace("{HEADER_STYLE}", &f_header_style);
		let f_html = f_html.replace("{HEADER_BODY}", &f_header_body);
		let f_html = f_html.replace("{HEADER_JS}", &f_header_js);

		std::fs::write(format!("../html/{}", file_name),f_html).unwrap();
	}
}