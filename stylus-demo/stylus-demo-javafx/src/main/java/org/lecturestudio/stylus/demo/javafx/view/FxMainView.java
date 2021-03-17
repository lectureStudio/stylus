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

package org.lecturestudio.stylus.demo.javafx.view;

import javafx.scene.Node;
import javafx.scene.layout.BorderPane;
import javafx.stage.Window;
import javafx.stage.WindowEvent;

import org.lecturestudio.stylus.demo.javafx.util.FxUtils;
import org.lecturestudio.stylus.demo.view.CanvasView;
import org.lecturestudio.stylus.demo.view.InfoView;
import org.lecturestudio.stylus.demo.view.MainView;
import org.lecturestudio.stylus.demo.view.ToolbarView;

public class FxMainView extends BorderPane implements MainView {

	@Override
	public void close() {
		// Fire close request in order to shutdown appropriately.
		Window window = getScene().getWindow();
		window.fireEvent(new WindowEvent(window, WindowEvent.WINDOW_CLOSE_REQUEST));
		window.hide();
	}

	@Override
	public void setToolbar(ToolbarView view) {
		FxUtils.checkNodeView(view);

		setTop((Node) view);
	}

	@Override
	public void setCanvas(CanvasView view) {
		FxUtils.checkNodeView(view);

		setCenter((Node) view);
	}

	@Override
	public void setInfo(InfoView view) {
		FxUtils.checkNodeView(view);

		setLeft((Node) view);
	}
}
