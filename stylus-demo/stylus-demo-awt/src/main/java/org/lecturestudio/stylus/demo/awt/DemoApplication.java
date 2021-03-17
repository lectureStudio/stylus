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
package org.lecturestudio.stylus.demo.awt;

import com.google.inject.Guice;
import com.google.inject.Injector;

import java.awt.BorderLayout;
import java.awt.Container;
import java.lang.System.Logger;
import java.lang.System.Logger.Level;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import org.lecturestudio.stylus.demo.awt.inject.AwtViewModule;
import org.lecturestudio.stylus.demo.presenter.MainPresenter;
import org.lecturestudio.stylus.demo.view.MainView;

public class DemoApplication {

	private static final Logger LOG = System.getLogger(DemoApplication.class.getName());


	private void start() {
		Injector injector = Guice.createInjector(new AwtViewModule());

		MainPresenter mainPresenter = injector.getInstance(MainPresenter.class);
		mainPresenter.initialize();

		MainView mainView = mainPresenter.getView();

		JFrame frame = new JFrame();
		frame.setTitle("AWT Stylus Demo");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add((Container) mainView, BorderLayout.CENTER);
		frame.setSize(800, 500);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}

	public static void main(String[] args) {
		SwingUtilities.invokeLater(() -> {
			try {
				DemoApplication app = new DemoApplication();
				app.start();
			}
			catch (Exception e) {
				LOG.log(Level.ERROR, "Create window failed.", e);
			}
		});
	}

}
