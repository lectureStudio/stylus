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

package org.lecturestudio.stylus.demo.beans;

import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

public abstract class ObservableBase<T> implements Observable<T> {

	private final Object bean;
	
	private ListenerManager<T> listenerManager;

	
	public ObservableBase() {
		this(null);
	}
	
	public ObservableBase(Object bean) {
		this.bean = bean;
	}
	
	@Override
	public void addListener(ChangeListener<? super T> listener) {
		if (isNull(listenerManager)) {
			listenerManager = new ListenerManager<>();
		}

		listenerManager.addListener(listener);
	}

	@Override
	public void removeListener(ChangeListener<? super T> listener) {
		if (nonNull(listenerManager)) {
			listenerManager.removeListener(listener);
		}
	}
	
	@Override
	public Object getBean() {
		return bean;
	}

	protected void fireChange(Observable<T> observable, T oldValue, T newValue) {
		if (nonNull(listenerManager)) {
			listenerManager.fireChange(observable, oldValue, newValue);
		}
	}
	
}
