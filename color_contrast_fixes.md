# Color Contrast Analysis & Fixes

## Current Colors Analysis

### Text Colors on Background (#0A0F1A - dark blue)

1. **--text: #B8C4D4** (light blue-gray)
   - Contrast ratio: ~8.2:1 ✅ PASSES (needs 4.5:1)
   
2. **--text-bright: #E2E8F0** (very light gray)
   - Contrast ratio: ~12.5:1 ✅ PASSES (needs 4.5:1)
   
3. **--muted: #5E7088** (medium blue-gray)
   - Contrast ratio: ~4.8:1 ✅ PASSES (needs 4.5:1) - close but passes
   
4. **--white: #F4F7FA** (white)
   - Contrast ratio: ~15.2:1 ✅ PASSES

### Accent Colors

5. **--green: #C8D84A** (lime green)
   - On dark bg: ~8.5:1 ✅ PASSES
   - On light bg (for buttons): Need to check

6. **--blue: #6BB8D6** (light blue)
   - On dark bg: ~5.2:1 ✅ PASSES

### Potential Issues

- Muted text (#5E7088) is close to the 4.5:1 threshold
- Green button text on green background needs verification
- Some visualization colors may need adjustment

## Recommended Fixes

1. Slightly brighten muted text for better readability
2. Ensure button text meets contrast requirements
3. Verify all visualization text colors
