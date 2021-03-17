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

package org.lecturestudio.stylus.demo.awt.inject;

import com.google.inject.AbstractModule;

import org.lecturestudio.stylus.demo.awt.view.AwtCanvasView;
import org.lecturestudio.stylus.demo.awt.view.AwtInfoView;
import org.lecturestudio.stylus.demo.awt.view.AwtMainView;
import org.lecturestudio.stylus.demo.awt.view.AwtToolbarView;
import org.lecturestudio.stylus.demo.view.CanvasView;
import org.lecturestudio.stylus.demo.view.InfoView;
import org.lecturestudio.stylus.demo.view.MainView;
import org.lecturestudio.stylus.demo.view.ToolbarView;

public class AwtViewModule extends AbstractModule {

	@Override
	protected void configure() {
		bind(CanvasView.class).to(AwtCanvasView.class);
		bind(InfoView.class).to(AwtInfoView.class);
		bind(MainView.class).to(AwtMainView.class);
		bind(ToolbarView.class).to(AwtToolbarView.class);
	}

}
