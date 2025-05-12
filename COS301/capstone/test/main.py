import cv2
import numpy as np
from skimage.morphology import skeletonize
import matplotlib.pyplot as plt
from math import atan2, cos, sin, pi, sqrt

def compute_tangent_angle(contour, point_idx, window=5):
    """Compute the tangent angle at a point using neighboring points"""
    n = len(contour)
    idx1 = max(0, point_idx - window)
    idx2 = min(n-1, point_idx + window)
    p1 = contour[idx1][0]
    p2 = contour[idx2][0]
    
    dx = p2[0] - p1[0]
    dy = p2[1] - p1[1]
    return atan2(dy, dx)

def find_boundary_intersection(point, angle, binary_mask, max_length):
    """Find where the line intersects with the track boundary"""
    step = 1
    x, y = point
    cos_a, sin_a = cos(angle), sin(angle)
    
    # Check in positive direction
    pos_length = 0
    while pos_length < max_length:
        pos_length += step
        px = int(x + pos_length * cos_a)
        py = int(y + pos_length * sin_a)
        if 0 <= px < binary_mask.shape[1] and 0 <= py < binary_mask.shape[0]:
            if binary_mask[py, px] == 0:  # Hit background
                pos_length -= step
                break
        else:
            pos_length -= step
            break
    
    # Check in negative direction
    neg_length = 0
    while neg_length > -max_length:
        neg_length -= step
        px = int(x + neg_length * cos_a)
        py = int(y + neg_length * sin_a)
        if 0 <= px < binary_mask.shape[1] and 0 <= py < binary_mask.shape[0]:
            if binary_mask[py, px] == 0:  # Hit background
                neg_length += step
                break
        else:
            neg_length += step
            break
    
    return (int(x + neg_length * cos_a), int(y + neg_length * sin_a)), \
           (int(x + pos_length * cos_a), int(y + pos_length * sin_a))

def draw_bounded_perpendicular_lines(img, skeleton, binary_mask, interval=20, max_length=50):
    """Draw perpendicular lines that stay within track boundaries"""
    contours, _ = cv2.findContours(skeleton, cv2.RETR_LIST, cv2.CHAIN_APPROX_NONE)
    
    if not contours:
        return img
    
    main_contour = max(contours, key=lambda x: cv2.arcLength(x, False))
    
    if len(img.shape) == 2:
        output = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
    else:
        output = img.copy()
    
    for i in range(0, len(main_contour), interval):
        point = main_contour[i][0]
        angle = compute_tangent_angle(main_contour, i)
        perp_angle = angle + pi/2
        
        # Find boundary intersections in both directions
        p1, p2 = find_boundary_intersection((point[0], point[1]), perp_angle, binary_mask, max_length)
        
        # Only draw if we found valid points
        if p1 and p2:
            cv2.line(output, p1, p2, (0, 0, 255), 1)
    
    return output

def process_track_image(image_path):
    # Load the image
    img = cv2.imread(image_path)
    if img is None:
        raise FileNotFoundError(f"Image not found at {image_path}")
    
    # Convert to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    
    # Threshold to create binary image (track as white, background as black)
    _, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)
    binary_mask = binary.copy()
    
    # Clean up the binary image
    kernel = np.ones((3,3), np.uint8)
    cleaned = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=2)
    
    # Ensure binary is boolean for skeletonize (0 or 1)
    binary_skeleton = (cleaned == 255).astype(np.uint8)
    
    # Perform skeletonization
    skeleton = skeletonize(binary_skeleton)
    skeleton_display = (skeleton * 255).astype(np.uint8)
    
    # Add bounded perpendicular lines
    result_with_ribs = draw_bounded_perpendicular_lines(img, skeleton_display, binary_mask, 
                                                      interval=30, max_length=100)
    
    return skeleton_display, result_with_ribs

def main():
    input_image = "track.png"
    
    try:
        skeleton, result = process_track_image(input_image)
        
        # Display results
        plt.figure(figsize=(12, 6))
        
        plt.subplot(1, 2, 1)
        plt.imshow(skeleton, cmap='gray')
        plt.title('Track Skeleton')
        
        plt.subplot(1, 2, 2)
        plt.imshow(cv2.cvtColor(result, cv2.COLOR_BGR2RGB))
        plt.title('Track with Bounded Perpendicular Lines')
        
        plt.tight_layout()
        plt.show()
        
        # Save the results
        cv2.imwrite('track_skeleton.png', skeleton)
        cv2.imwrite('track_with_bounded_ribs.png', result)
        print("Results saved as 'track_skeleton.png' and 'track_with_bounded_ribs.png'")
        
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()