import cv2
import numpy as np
from skimage.morphology import skeletonize, remove_small_objects
from skimage.filters import gaussian

def extract_track_mask(image_path):
    img = cv2.imread(image_path)
    if img is None:
        raise ValueError(f"Could not load image from {image_path}")

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # Threshold for dark gray road (low saturation, low brightness)
    lower = np.array([0, 0, 30])
    upper = np.array([180, 70, 130])
    mask = cv2.inRange(hsv, lower, upper)

    # Morphological cleanup
    kernel = np.ones((5, 5), np.uint8)
    cleaned = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel, iterations=2)
    cleaned = cv2.morphologyEx(cleaned, cv2.MORPH_OPEN, kernel, iterations=1)

    return cleaned


def extract_centerline(track_mask, smooth=True):
    if track_mask.max() == 0:  # Empty mask
        return np.zeros_like(track_mask, dtype=np.uint8)
        
    # Skeletonize
    skeleton = skeletonize(track_mask > 0).astype(np.uint8)
    
    # Optional smoothing
    if smooth:
        skeleton = gaussian(skeleton, sigma=0.5)
        skeleton = (skeleton > 0.5).astype(np.uint8)
    
    # Remove small artifacts
    skeleton = remove_small_objects(skeleton.astype(bool), min_size=20).astype(np.uint8)
    
    return skeleton * 255  # Return as 0-255 image