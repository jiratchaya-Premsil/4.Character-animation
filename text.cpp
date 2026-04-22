
#include <iostream>
#include <glad/glad.h>
#include "text.h"
#include "stb_image.h"
#include <cctype>
const int spacing_Letter = 5;

float widthPx = 33.0;
float highPx = 39.0;
int borderPx = 5;

int picwidth = 501;
int pichight = 90;

std::map <char, Glyph> fontMap;

std::string fontPath = "resources/fonts/customeFontBitmap.png";

void TextRenderer::init() {
	//load texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int w, h, ch;
	unsigned char* data = stbi_load(fontPath.c_str(), &w, &h, &ch, 4);
	if (!data) {
		std::cout << "failed to load texture" << std::endl;
		return;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	// setup quad buffer (dynamic)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// simple grid atlas (16x16)
	

	float x = 0 + borderPx;
	float y = 0 + borderPx;
	float endX = x + widthPx;
	float endY = y + highPx;

	int columns = 13;

	for (int i = 0; i < 26; i++)
	{
		int col = i % columns;   // 0 → 12
		int row = i / columns;   // 0 or 1

		float x = borderPx + col * (widthPx + borderPx);
		float y = borderPx + row * (highPx +(row * borderPx));

		float endX = x + widthPx;
		float endY = y + highPx;

		Glyph g;
		g.u1 = x / picwidth;
		g.v1 = y / pichight;
		g.u2 = endX / picwidth;
		g.v2 = endY / pichight;

		fontMap[(char)(i + 65)] = g;
	}

}


void TextRenderer::drawText(std::string text, float x, float y, float scale) {
	float originx = x;
	for (char& c : text) {
		c = std::toupper(static_cast<unsigned char>(c));
	}

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	for (char c : text) {
		if (c == ' ') {
					x += widthPx;
					continue;
				}
		if (c == '_') {
			x = originx;
			y += (highPx * scale) + 10;
			continue;
		}
		if (fontMap.find(c) == fontMap.end()) {
			std::cout << "unable to find letter " << c << std::endl;
		}
		
		

		Glyph g = fontMap[c];
		float w = widthPx * scale;
		float h = highPx * scale;
		float vertices[] = {
			x,     y,     g.u1, g.v1,
			x + w,   y,     g.u2, g.v1,
			x + w,   y + h,   g.u2, g.v2,

			x,     y,     g.u1, g.v1,
			x + w,   y + h,   g.u2, g.v2,
			x,     y + h,   g.u1, g.v2
		};
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += w + spacing_Letter;


	}


}