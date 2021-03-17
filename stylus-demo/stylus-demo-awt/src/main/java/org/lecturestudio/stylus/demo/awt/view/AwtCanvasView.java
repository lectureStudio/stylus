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

package org.lecturestudio.stylus.demo.awt.view;

import static java.util.Objects.nonNull;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import javax.swing.ImageIcon;
import javax.swing.JPanel;

import org.lecturestudio.stylus.StylusCursor;
import org.lecturestudio.stylus.StylusListener;
import org.lecturestudio.stylus.awt.AwtStylusManager;
import org.lecturestudio.stylus.demo.awt.render.PenRenderer;
import org.lecturestudio.stylus.demo.awt.util.AwtUtils;
import org.lecturestudio.stylus.demo.model.Shape;
import org.lecturestudio.stylus.demo.model.StrokeShape;
import org.lecturestudio.stylus.demo.render.RenderSurfaceRenderer;
import org.lecturestudio.stylus.demo.view.Action;
import org.lecturestudio.stylus.demo.view.CanvasView;

public class AwtCanvasView extends JPanel implements CanvasView {

	private static final GraphicsEnvironment GE = GraphicsEnvironment.getLocalGraphicsEnvironment();
	private static final GraphicsConfiguration GC = GE.getDefaultScreenDevice().getDefaultConfiguration();

	private Map<Class<? extends Shape>, RenderSurfaceRenderer<? extends Shape, Graphics2D>> renderers;

	private Cursor stylusCursor;

	private BufferedImage bufferImage;

	private Graphics2D graphics2D;

	private Action resizeAction;


	public AwtCanvasView() {
		super();

		initialize();
	}

	@Override
	public void setStylusCursor(StylusCursor cursor) {
		AwtUtils.invoke(() -> {
			switch (cursor) {
				case PEN:
				case ERASER:
					setCursor(stylusCursor);
					break;

				default:
					setCursor(Cursor.getDefaultCursor());
					break;
			}
		});
	}

	@Override
	public void setStylusListener(StylusListener listener) {
		AwtStylusManager manager = AwtStylusManager.getInstance();
		manager.attachStylusListener(this, listener);
	}

	@Override
	public void setOnResize(Action action) {
		this.resizeAction = action;
	}

	@Override
	public void clear() {
		graphics2D.setPaint(Color.WHITE);
		graphics2D.fillRect(0, 0, bufferImage.getWidth(), bufferImage.getHeight());
	}

	@Override
	public <S extends Shape> void render(S shape) {
		AwtUtils.invoke(() -> {
			renderShape(shape);

			repaint();
		});
	}

	@Override
	public <S extends Shape> void render(Collection<S> shapes) {
		AwtUtils.invoke(() -> {
			shapes.forEach(this::renderShape);

			repaint();
		});
	}

	@Override
	public <S extends Shape> void rerender(Collection<S> shapes) {
		AwtUtils.invoke(() -> {
			clear();

			shapes.forEach(this::renderShape);

			repaint();
		});
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);

		if (nonNull(bufferImage)) {
			/*
			 * HiDPI scaling:
			 * Set to Identity transform, since the renderer scales the image
			 * to the appropriate size.
			 */
			Graphics2D g2d = (Graphics2D) g;
			final AffineTransform transform = g2d.getTransform();
			final AffineTransform imageTransform = new AffineTransform();
			imageTransform.translate(transform.getTranslateX(), transform.getTranslateY());

			g2d.setTransform(imageTransform);
			g2d.drawImage(bufferImage, 0, 0, this);
			g2d.setTransform(transform);
		}
	}

	@SuppressWarnings("unchecked")
	private <S extends Shape> void renderShape(S shape) {
		RenderSurfaceRenderer<S, Graphics2D> renderer = (RenderSurfaceRenderer<S, Graphics2D>) getRenderer(shape.getClass());
		renderer.render(shape, graphics2D);
	}

	private RenderSurfaceRenderer<? extends Shape, Graphics2D> getRenderer(Class<? extends Shape> cls) {
		return renderers.get(cls);
	}

	private <S extends Shape> void setRenderer(Class<S> cls, RenderSurfaceRenderer<S, Graphics2D> renderer) {
		renderers.put(cls, renderer);
	}

	private void createBufferImage() {
		AffineTransform t = getGraphicsConfiguration().getDefaultTransform();

		int width = (int) (getWidth() * t.getScaleX());
		int height = (int) (getHeight() * t.getScaleY());

		if (width < 1 || height < 1) {
			return;
		}
		if (nonNull(bufferImage)) {
			if (width == bufferImage.getWidth() && height == bufferImage.getHeight()) {
				return;
			}

			// Size does not match. Release resources.
			if (nonNull(graphics2D)) {
				graphics2D.dispose();
			}

			bufferImage.flush();
		}

		bufferImage = GC.createCompatibleImage(width, height);
		bufferImage.setAccelerationPriority(1);

		graphics2D = bufferImage.createGraphics();
		graphics2D.scale(t.getScaleX(), t.getScaleY());
	}

	private void onResize() {
		if (nonNull(resizeAction)) {
			resizeAction.execute();
		}
	}

	private void initialize() {
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		Dimension cursorBestSize = toolkit.getBestCursorSize(32, 32);
		int cursorSize = cursorBestSize.width;
		Point hotspot = new Point(cursorSize / 2, cursorSize / 2);
		Image image = new ImageIcon(getClass().getResource("/cursor.png")).getImage();

		stylusCursor = toolkit.createCustomCursor(image, hotspot, null);

		renderers = new HashMap<>();

		setRenderer(StrokeShape.class, new PenRenderer());

		addComponentListener(new ComponentAdapter() {

			@Override
			public void componentResized(ComponentEvent e) {
				createBufferImage();
				onResize();
			}
		});
	}
}
