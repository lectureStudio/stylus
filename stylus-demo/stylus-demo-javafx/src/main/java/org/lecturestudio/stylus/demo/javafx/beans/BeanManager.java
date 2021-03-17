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

import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javafx.beans.property.Property;

public class BeanManager {

	private final Map<Property<?>, Set<Property<?>>> propertyMap = new HashMap<>();


	public <T> void bindBidirectional(Property<T> a, Property<T> b) {
		a.bindBidirectional(b);

		Set<Property<?>> set = propertyMap.get(a);

		if (isNull(set)) {
			set = new HashSet<>();

			propertyMap.put(a, set);
		}

		set.add(b);
	}

	@SuppressWarnings("unchecked")
	public <T> void unbindBidirectional(Property<T> property) {
		Set<Property<?>> set = propertyMap.get(property);

		if (nonNull(set)) {
			for (Property<?> p : set) {
				property.unbindBidirectional((Property<T>) p);
			}
		}
	}
}
