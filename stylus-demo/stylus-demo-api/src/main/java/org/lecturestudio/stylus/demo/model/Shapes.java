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

package org.lecturestudio.stylus.demo.model;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import javax.inject.Singleton;

@Singleton
public class Shapes {

	private List<Shape> shapes = new ArrayList<>();

	private List<ShapesChangeListener> listeners = new ArrayList<>();


	public boolean add(Shape shape) {
		boolean added = shapes.add(shape);

		if (added) {
			fireAdded(shape);
		}

		return added;
	}

	public boolean addAll(Collection<Shape> collection) {
		boolean added = shapes.addAll(collection);

		if (added) {
			fireAdded(collection);
		}

		return added;
	}

	public boolean remove(Shape shape) {
		boolean removed = shapes.remove(shape);

		if (removed) {
			fireRemoved(shape);
		}

		return removed;
	}

	public void clear() {
		int size = shapes.size();

		shapes.clear();

		if (size != shapes.size()) {
			fireRemoved(shapes);
		}
	}

	public List<Shape> getShapes() {
		return shapes;
	}

	public void addShapesChangeListener(ShapesChangeListener listener) {
		listeners.add(listener);
	}

	private void fireAdded(Shape shape) {
		for (ShapesChangeListener listener : listeners) {
			listener.onShapeInserted(this, shape);
		}
	}

	private void fireAdded(Collection<Shape> list) {
		for (ShapesChangeListener listener : listeners) {
			listener.onShapesInserted(this, list);
		}
	}

	private void fireRemoved(Shape shape) {
		for (ShapesChangeListener listener : listeners) {
			listener.onShapeRemoved(this, shape);
		}
	}

	private void fireRemoved(Collection<Shape> list) {
		for (ShapesChangeListener listener : listeners) {
			listener.onShapesRemoved(this, list);
		}
	}
}
