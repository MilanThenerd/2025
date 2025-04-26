import cv2
import numpy as np
from track_analysis.extract_track import extract_track_mask, extract_centerline

def test_thresholds(image_path, start_thresh=50, end_thresh=250, step=10):
    original = cv2.imread(image_path)
    
    for threshold in range(start_thresh, end_thresh, step):
        print(f"Testing threshold: {threshold}")
        
        mask = extract_track_mask(image_path)
        cv2.imshow("Track Mask", track_mask)
        cv2.waitKey(0)
        centerline = extract_centerline(mask)
        
        # Combine images for display
        combined = np.hstack((
            original,
            cv2.cvtColor(mask, cv2.COLOR_GRAY2BGR),
            cv2.cvtColor(centerline, cv2.COLOR_GRAY2BGR)
        ))
        
        # Add threshold text to image
        cv2.putText(combined, f"Threshold: {threshold}", (10, 30),
                   cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        
        cv2.imshow("Threshold Testing", combined)
        
        # Press 'q' to quit or any other key for next threshold
        if cv2.waitKey(0) & 0xFF == ord('q'):
            break
    
    cv2.destroyAllWindows()

def main():
    image_path = "/home/milan/2025/COS301/capstone/test/track_images/image.jpg"
    test_thresholds(image_path, 0, 251, 2)  # Tests thresholds from 50 to 250 in steps of 10

if __name__ == "__main__":
    main()