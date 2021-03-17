/*
 * Copyright 2016 Alex Andres
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lecturestudio.stylus.demo.javafx.beans.converter;

import javafx.scene.paint.Color;

import org.lecturestudio.stylus.demo.beans.converter.Converter;

public class ColorConverter implements Converter<org.lecturestudio.stylus.demo.paint.Color, Color> {

	public static final ColorConverter INSTANCE = new ColorConverter();


	@Override
	public Color to(org.lecturestudio.stylus.demo.paint.Color color) {
		return Color.rgb(color.getRed(), color.getGreen(), color.getBlue(), color.getOpacity() / 255.0);
	}

	@Override
	public org.lecturestudio.stylus.demo.paint.Color from(Color color) {
		int r = (int) Math.round(color.getRed() * 255);
		int g = (int) Math.round(color.getGreen() * 255);
		int b = (int) Math.round(color.getBlue() * 255);
		int a = (int) Math.round(color.getOpacity() * 255);

		return new org.lecturestudio.stylus.demo.paint.Color(r, g, b, a);
	}

}
