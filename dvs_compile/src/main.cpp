#include <string>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <random>

void ProcessImageB(const std::string& file, sf::RenderTexture& txB, sf::Texture& texture, sf::RectangleShape& shape)
{
	printf(" [ Processing big image ] %s\n", file.c_str());
	texture.loadFromFile("pic/" + file + ".png");

	shape.setTexture(&texture);
	txB.draw(shape);

	sf::Image ni = txB.getTexture().copyToImage();
	ni.flipVertically();

	for (uint64_t i = 0; i < ni.getSize().x; i++)
		for (uint64_t a = 0; a < ni.getSize().y; a++)
			ni.setPixel(i, a, ni.getPixel(i, a));

	ni.saveToFile("build/" + file + "_b_h.png");

	for (uint64_t i = 0; i < ni.getSize().x; i++)
		for (uint64_t a = 0; a < ni.getSize().y; a++)
		{
			uint8_t adverage = std::floor(((ni.getPixel(i, a).r / 64.0) + (ni.getPixel(i, a).g / 64.0) + (ni.getPixel(i, a).b / 64.0)) / 3.0) * 64;
			ni.setPixel(i, a, sf::Color(adverage, adverage, adverage, 255));
		}

	ni.saveToFile("build/" + file + "_b.png");
}

void ProcessImageS(const std::string& file, sf::RenderTexture& txS, sf::Texture& texture, sf::RectangleShape& shape)
{
	printf(" [ Processing small image ] %s\n", file.c_str());
	texture.loadFromFile("pic/" + file + ".png");

	shape.setTexture(&texture);
	txS.draw(shape);

	sf::Image ni = txS.getTexture().copyToImage();
	ni.flipVertically();

	for (uint64_t i = 0; i < ni.getSize().x; i++)
		for (uint64_t a = 0; a < ni.getSize().y; a++)
			ni.setPixel(i, a, sf::Color(
				std::floor(ni.getPixel(i, a).r / 8.0) * 8,
				std::floor(ni.getPixel(i, a).g / 8.0) * 8,
				std::floor(ni.getPixel(i, a).b / 8.0) * 8,
				255));

	ni.saveToFile("build/" + file + "_s_h.png");

	for (uint64_t i = 0; i < ni.getSize().x; i++)
		for (uint64_t a = 0; a < ni.getSize().y; a++)
		{
			uint8_t adverage = std::floor(((ni.getPixel(i, a).r / 64.0) + (ni.getPixel(i, a).g / 64.0) + (ni.getPixel(i, a).b / 64.0)) / 3.0) * 64;
			ni.setPixel(i, a, sf::Color(adverage, adverage, adverage, 255));
		}

	ni.saveToFile("build/" + file + "_s.png");
}

void ImageProcess()
{
	std::vector<std::string> files;

	std::filesystem::directory_iterator pathMods("pic");
	for (auto& file : pathMods)
		files.push_back(file.path().stem().string());

	printf("########### Processing full images\n");

	for (uint64_t i = 0; i < files.size(); i++)
	{
		printf(" [ Processing Full Image ] %s\n", files[i].c_str());
		std::filesystem::copy_file("pic/" + files[i] + ".png", "build/" + files[i] + "_a.png");
	}

	{
		sf::Texture texture;

		sf::RenderTexture txB;
		txB.create(1280, 720);
		

		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(1280, 720));

		printf("########### Processing big images\n");

		for (uint64_t i = 0; i < files.size(); i++)
			ProcessImageB(files[i], txB, texture, shape);
	}

	{
		sf::Texture texture;

		sf::RenderTexture txS;
		txS.create(480, 270, sf::ContextSettings(2, 2));

		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(480, 270));

		printf("########### Processing small images\n");

		for (uint64_t i = 0; i < files.size(); i++)
			ProcessImageS(files[i], txS, texture, shape);
	}
}

enum class UserStatus
{
	Owner, Admin, Moderator, Member
};

class UserData
{
public:
	std::string UserName;
	std::string Quote;
	
	UserStatus UserStatus;

	std::vector<std::string> ImageFiles;
	std::vector<std::string> ImageTitles;
};

static std::vector<UserData> data;

bool ProcessIndexUsers()
{
	printf("########### Loading users\n");

	data.clear();

	std::filesystem::directory_iterator pathMods("users");
	for (auto& file : pathMods)
	{
		data.push_back(UserData());
		
		std::ifstream stream;
		std::string line;
		std::vector<std::string> lines;

		printf(" [ Loading user ] %s\n", file.path().stem().string().c_str());

		stream.open("users/" + file.path().stem().string() + ".txt");

		while (std::getline(stream, line))
			lines.push_back(line);

		stream.close();

		if (lines.size() < 3)
		{
			printf("error processing user \"%s\"; there must be atleast 3 lines of data!\n", file.path().stem().string().c_str());
			return false;
		}

		data.back().UserName = lines[0];
		data.back().Quote = lines[1];

		if (lines[2] == "owner")
			data.back().UserStatus = UserStatus::Owner;
		else if (lines[2] == "admin")
			data.back().UserStatus = UserStatus::Admin;
		else if (lines[2] == "moderator")
			data.back().UserStatus = UserStatus::Moderator;
		else if (lines[2] == "member")
			data.back().UserStatus = UserStatus::Member;
		else
		{
			printf("error processing user \"%s\"; invalid user status \"%s\"\n", file.path().stem().string().c_str(), lines[2].c_str());
			return false;
		}

		uint64_t i = 3;

		while (lines[i] != ";")
		{
			data.back().ImageFiles.push_back(lines[i]);
			i++;

			if (i >= lines.size())
			{
				printf("error processing user \"%s\"; failed to parse image files\n", file.path().stem().string().c_str());
				return false;
			}
		}

		i++;

		while (lines[i] != ";")
		{
			data.back().ImageTitles.push_back(lines[i]);
			i++;

			if (i >= lines.size())
			{
				printf("error processing user \"%s\"; failed to parse image titles\n", file.path().stem().string().c_str());
				return false;
			}
		}

		if (data.back().ImageFiles.size() != data.back().ImageTitles.size())
		{
			printf("error processing user \"%s\"; you must have the same number of image files as you do image titles\n", file.path().stem().string().c_str());
			return false;
		}
	}

	printf("########### Building index\n");

	std::vector<std::string> indexLines;

	{
		std::string line;
		std::ifstream stream;

		stream.open("pre/index_begin.html");
		while (std::getline(stream, line))
			indexLines.push_back(line);

		stream.close();

		for (uint64_t i = 0; i < data.size(); i++)
		{
			printf(" [ Building index user ] %s\n", data[i].UserName.c_str());

			std::string buildLine;
			buildLine.reserve(150);

			buildLine += "			<h3 class=\"";

			if (data[i].UserStatus == UserStatus::Owner)
				buildLine += "tag_owner";
			else if (data[i].UserStatus == UserStatus::Admin)
				buildLine += "tag_admin";
			else if (data[i].UserStatus == UserStatus::Moderator)
				buildLine += "tag_mod";
			else if (data[i].UserStatus == UserStatus::Member)
				buildLine += "tag_member";

			buildLine += "\" onmousedown = \"openUser('";
			buildLine += data[i].UserName;
			buildLine += "')\" style = \"cursor: pointer;\" > [ ";

			if (data[i].UserStatus == UserStatus::Owner)
				buildLine += "Owner";
			else if (data[i].UserStatus == UserStatus::Admin)
				buildLine += "Admin";
			else if (data[i].UserStatus == UserStatus::Moderator)
				buildLine += "Moderator";
			else if (data[i].UserStatus == UserStatus::Member)
				buildLine += "Member";

			buildLine += " ] ";
			buildLine += data[i].UserName;
			buildLine += "</h3>";

			indexLines.push_back(buildLine);
		}
	}

	{
		std::string line;
		std::ifstream stream;

		stream.open("pre/index_end.html");
		while (std::getline(stream, line))
			indexLines.push_back(line);

		stream.close();
	}

	{
		std::ofstream stream;
		stream.open("build/index.html");

		for (uint64_t i = 0; i < indexLines.size(); i++)
			stream << indexLines[i] + "\n";

		stream.close();
	}

	return true;
}

bool ProcessUsers()
{
	for (uint64_t i = 0; i < data.size(); i++)
	{
		std::vector<std::string> userLines;

		{
			std::string line;
			std::ifstream stream;

			stream.open("pre/user_begin.html");
			while (std::getline(stream, line))
				userLines.push_back(line);

			stream.close();
		}

		userLines.push_back("const rand = " + std::to_string(std::rand() % 10000) + ";");
		userLines.push_back("const pictures = [");

		for (uint64_t a = 0; a < data[i].ImageFiles.size(); a++)
			userLines.push_back("	\"" + data[i].ImageFiles[a] + "\",");

		userLines.push_back("];");
		userLines.push_back("const names = [");

		for (uint64_t a = 0; a < data[i].ImageTitles.size(); a++)
			userLines.push_back("	\"" + data[i].ImageTitles[a] + "\",");

		userLines.push_back("];");

		{
			std::string line;
			std::ifstream stream;

			stream.open("pre/user_middle.html");
			while (std::getline(stream, line))
				userLines.push_back(line);

			stream.close();
		}

		{
			std::string buildLine;
			buildLine.reserve(80);

			buildLine += "<h1 class=\"";

			if (data[i].UserStatus == UserStatus::Owner)
				buildLine += "tag_owner";
			else if (data[i].UserStatus == UserStatus::Admin)
				buildLine += "tag_admin";
			else if (data[i].UserStatus == UserStatus::Moderator)
				buildLine += "tag_mod";
			else if (data[i].UserStatus == UserStatus::Member)
				buildLine += "tag_member";

			buildLine += "\"> [ ";

			if (data[i].UserStatus == UserStatus::Owner)
				buildLine += "Owner";
			else if (data[i].UserStatus == UserStatus::Admin)
				buildLine += "Admin";
			else if (data[i].UserStatus == UserStatus::Moderator)
				buildLine += "Moderator";
			else if (data[i].UserStatus == UserStatus::Member)
				buildLine += "Member";

			buildLine += " ] ";
			buildLine += data[i].UserName;
			buildLine += "</h1>";

			userLines.push_back(buildLine);
		}

		userLines.push_back("<p style=\"width: 500px\">");
		userLines.push_back("	" + data[i].Quote);

		if (data[i].ImageFiles.size() == 0)
			userLines.push_back("</p><table><tr valign=\"top\"><td><div id=\"pic\"></div></td><td><img src=\"blank.png\" id=\"big\"></img></td></tr></table></body></html>");
		else
			userLines.push_back("</p><table><tr valign=\"top\"><td><div id=\"pic\"></div></td><td><img src=\"" + data[i].ImageFiles[0] + "_d_b.png\" id=\"big\"></img></td></tr></table></body></html>");

		{
			std::ofstream stream;
			stream.open("build/" + data[i].UserName + ".html");

			for (uint64_t a = 0; a < userLines.size(); a++)
				stream << userLines[a] + "\n";

			stream.close();
		}
	}

	return true;
}

int main()
{
	std::filesystem::remove_all("build");
	std::filesystem::create_directory("build");

	std::filesystem::copy_file("blank.png", "build/blank.png");

	printf("########### Processing images\n");
	ImageProcess();

	printf("########### Processing index users\n");
	if (!ProcessIndexUsers())
	{
		printf("there was an error processing index users, this session will be halted!\n");
		system("pause");
		return -1;
	}

	printf("########### Processing users\n");
	if (!ProcessUsers())
	{
		printf("there was an error processing users, this session will be halted!\n");
		system("pause");
		return -1;
	}
}