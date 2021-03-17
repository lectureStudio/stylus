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

import javafx.beans.property.ObjectPropertyBase;

import org.lecturestudio.stylus.demo.beans.ChangeListener;
import org.lecturestudio.stylus.demo.beans.ObjectProperty;
import org.lecturestudio.stylus.demo.beans.converter.Converter;

public class ConvertibleObjectProperty<S, T> extends ObjectPropertyBase<T> {

	private final ChangeListener<S> changeListener;

	protected final ObjectProperty<S> wrappedProperty;
	
	protected final Converter<S, T> converter;


	public ConvertibleObjectProperty(ObjectProperty<S> property, Converter<S, T> converter) {
		this.wrappedProperty = property;
		this.converter = converter;

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
	public T get() {
		return converter.to(wrappedProperty.get());
	}

	@Override
	public void set(T value) {
		wrappedProperty.set(converter.from(value));
	}

	@Override
	public void unbind() {
		super.unbind();

		if (nonNull(wrappedProperty)) {
			wrappedProperty.removeListener(changeListener);
		}
	}

}
