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

package org.lecturestudio.stylus.demo.action;

import static java.util.Objects.nonNull;

import java.util.Stack;

import javax.inject.Inject;
import javax.inject.Singleton;

import org.lecturestudio.stylus.demo.model.Shapes;

@Singleton
public class ActionController {

	private final Shapes shapes;

	private Runnable changeAction;

	private final Stack<Action> undoStack;

	private final Stack<Action> redoStack;


	@Inject
	public ActionController(Shapes shapes) {
		this.shapes = shapes;

		undoStack = new Stack<>();
		redoStack = new Stack<>();
	}

	public boolean canUndo() {
		return !undoStack.isEmpty();
	}

	public boolean canRedo() {
		return !redoStack.isEmpty();
	}

	public void setOnAction(Runnable action) {
		this.changeAction = action;
	}

	public void execute(ShapeAction action) {
		redoStack.clear();

		action.setShapes(shapes);
		action.execute();

		undoStack.add(action);

		fireActionExecuted();
	}

	public void undo() {
		if (!canUndo()) {
			return;
		}

		Action action = undoStack.pop();
		action.undo();

		redoStack.push(action);

		fireActionExecuted();
	}

	public void redo() {
		if (!canRedo()) {
			return;
		}

		Action action = redoStack.pop();
		action.redo();

		undoStack.push(action);

		fireActionExecuted();
	}

	private void fireActionExecuted() {
		if (nonNull(changeAction)) {
			changeAction.run();
		}
	}
}
