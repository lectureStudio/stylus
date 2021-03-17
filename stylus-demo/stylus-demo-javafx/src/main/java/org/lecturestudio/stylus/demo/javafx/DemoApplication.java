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

package org.lecturestudio.stylus.demo.javafx;

import com.google.inject.Guice;
import com.google.inject.Injector;

import javafx.application.Application;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import org.lecturestudio.stylus.demo.javafx.inject.FxViewModule;
import org.lecturestudio.stylus.demo.presenter.MainPresenter;
import org.lecturestudio.stylus.demo.view.MainView;

public class DemoApplication extends Application {

	@Override
	public void start(Stage primaryStage) {
		Injector injector = Guice.createInjector(new FxViewModule());

		MainPresenter mainPresenter = injector.getInstance(MainPresenter.class);
		mainPresenter.initialize();

		MainView mainView = mainPresenter.getView();

		Scene scene = new Scene((Parent) mainView, 800, 500);

		primaryStage.setTitle("JavaFX Stylus Demo");
		primaryStage.setScene(scene);
		primaryStage.show();
		primaryStage.setOnCloseRequest(event -> System.exit(0));
	}

	public static void main(String[] args) {
		launch(args);
	}
}
