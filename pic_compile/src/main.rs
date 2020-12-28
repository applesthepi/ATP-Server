use image::io::Reader as ImageReader;

use std::fs;

fn main() {
    let paths = fs::read_dir("images").unwrap();

    for path in paths {
        let mut image = ImageReader::open(path.unwrap().path())
            .unwrap()
            .decode()
            .unwrap();

        let image_data = image.as_rgb8().unwrap();

        let mut image_cut: u32 = 0;

        let mut image_pixel_r: u8 = 0;
        let mut image_pixel_g: u8 = 0;
        let mut image_pixel_b: u8 = 0;

        while image_pixel_r == 0 && image_pixel_g == 0 && image_pixel_b == 0 {
            let segment = image_data.get_pixel(image_cut, 0);

            image_pixel_r = segment[0];
            image_pixel_g = segment[1];
            image_pixel_b = segment[2];

            image_cut += 1;
        }

        let mut cropped_image: image::DynamicImage = image::DynamicImage::new_rgb8(
            image_data.width() - (image_cut * 2),
            image_data.height(),
        );

        cropped_image.clone_from(&image);
        cropped_image.crop(
            image_cut,
            0,
            image_data.width() - (image_cut * 2),
            image_data.height(),
        );
        cropped_image.save("test.png").unwrap();
    }
}
