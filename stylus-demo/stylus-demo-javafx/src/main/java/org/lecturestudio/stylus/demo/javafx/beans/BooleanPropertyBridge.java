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

package org.lecturestudio.stylus.demo.javafx.beans;

import static java.util.Objects.nonNull;

import javafx.beans.property.BooleanPropertyBase;

import org.lecturestudio.stylus.demo.beans.BooleanProperty;
import org.lecturestudio.stylus.demo.beans.ChangeListener;

public class BooleanPropertyBridge extends BooleanPropertyBase {

	private final BooleanProperty wrappedProperty;

	private final ChangeListener<Boolean> changeListener;


	public BooleanPropertyBridge(BooleanProperty property) {
		this.wrappedProperty = property;

		changeListener = (observable, oldValue, newValue) -> {
			invalidated();
			fireValueChangedEvent();
		};

		wrappedProperty.addListener(changeListener);
	}

	@Override
	public Object getBean() {
		return null;
	}

	@Override
	public String getName() {
		return null;
	}

	@Override
	public boolean get() {
		return wrappedProperty.get();
	}

	@Override
	public void set(boolean value) {
		wrappedProperty.set(value);
	}

	@Override
	public void unbind() {
		super.unbind();

		if (nonNull(wrappedProperty)) {
			wrappedProperty.removeListener(changeListener);
		}
	}

}
