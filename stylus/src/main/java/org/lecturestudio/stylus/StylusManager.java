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

package org.lecturestudio.stylus;

import java.lang.System.Logger;
import java.lang.System.Logger.Level;

import org.lecturestudio.stylus.internal.NativeLoader;

public class StylusManager {

	private static final Logger LOG = System.getLogger(StylusManager.class.getName());

	static {
		try {
			NativeLoader.loadLibrary("stylus");
		}
		catch (Exception e) {
			LOG.log(Level.ERROR, "Load stylus library failed", e);
		}
	}


	native private void destroy();
	native public void attachStylusListener(StylusListener listener, Object window);
	native public void detachStylusListener(StylusListener listener, Object window);
	native public void enableStylusListener(StylusListener listener, boolean enable);
	native public StylusDevice[] getDevices();


	private StylusManager() {
		Runtime.getRuntime().addShutdownHook(new Thread(this::dispose));
	}

	public void dispose() {
		try {
			destroy();
		}
		catch (Exception e) {
			LOG.log(Level.ERROR, "Destroy stylus manager failed", e);
		}
	}

	public static StylusManager getInstance() {
		return InstanceHolder.INSTANCE;
	}

	private static final class InstanceHolder {

		static final StylusManager INSTANCE = init();

		private static StylusManager init() {
			return new StylusManager();
		}
	}

}
