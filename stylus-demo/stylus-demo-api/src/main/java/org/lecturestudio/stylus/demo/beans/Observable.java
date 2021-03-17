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

public interface Observable<T> {

	/**
	 * Adds the given listener in order to be notified whenever the value of
	 * this Observable changes.
	 *
	 * @param listener The listener to register.
	 * 
	 * @throws NullPointerException if the listener is null.
	 */
    void addListener(ChangeListener<? super T> listener);

	/**
	 * Removes the given listener from the list of listeners. This method call
	 * has no effect if the given listener has not been previously registered.
	 *
	 * @param listener The listener to remove.
	 * 
	 * @throws NullPointerException if the listener is null.
	 */
	void removeListener(ChangeListener<? super T> listener);
	
	/**
	 * Returns the Object that contains this Observable. If this Observable is not
	 * contained in an Object, null is returned.
	 * 
	 * @return The Object that contains this Observable or null.
	 */
	Object getBean();
	
}
