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

package org.lecturestudio.stylus.demo.awt.beans.converter;

import java.awt.Color;

import org.lecturestudio.stylus.demo.beans.converter.Converter;

public class ColorConverter implements Converter<org.lecturestudio.stylus.demo.paint.Color, Color> {

	public static final ColorConverter INSTANCE = new ColorConverter();


	@Override
	public Color to(org.lecturestudio.stylus.demo.paint.Color c) {
		return new java.awt.Color(c.getRGBA(), true);
	}

	@Override
	public org.lecturestudio.stylus.demo.paint.Color from(Color c) {
		return new org.lecturestudio.stylus.demo.paint.Color(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
	}

}
