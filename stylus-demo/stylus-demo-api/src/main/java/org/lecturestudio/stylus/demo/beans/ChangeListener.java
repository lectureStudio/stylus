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

import java.util.Objects;

@FunctionalInterface
public interface ChangeListener<T> {

    /**
     * This method is called if the value of an {@code Observable} has changed.
     *
     * @param observable The Observable of which the value changed.
     * @param oldValue The old value.
     * @param newValue The new value.
     */
    void changed(Observable<? extends T> observable, T oldValue, T newValue);

	/**
	 * Returns a composed {@code ChangeListener} that performs, in sequence, this
	 * operation followed by the {@code after} operation. If performing either
	 * operation throws an exception, it is relayed to the caller of the
	 * composed operation. If performing this operation throws an exception,
	 * the {@code after} operation will not be performed.
	 *
	 * @param after the operation to perform after this operation.
	 *
	 * @return a composed {@code ChangeListener} that performs in sequence this
	 * operation followed by the {@code after} operation.
	 *
	 * @throws NullPointerException if {@code after} is null.
	 */
	default ChangeListener<T> andThen(ChangeListener<? super T> after) {
		Objects.requireNonNull(after);

		return (Observable<? extends T> observable, T oldValue, T newValue) -> {
			changed(observable, oldValue, newValue);
			after.changed(observable, oldValue, newValue);
		};
	}
}
