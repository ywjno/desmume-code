/*
	Copyright (C) 2011 Roger Manuel
	Copyright (C) 2011-2015 DeSmuME team

	This file is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with the this software.  If not, see <http://www.gnu.org/licenses/>.
*/

#import <Foundation/Foundation.h>
#include <pthread.h>
#include <libkern/OSAtomic.h>

#import "cocoa_util.h"

@class NSImage;
@class NSBitmapImageRep;

struct NDSFrameInfo;

typedef struct
{
	double	scale;
	double	rotation;		// Angle is in degrees
	double	translationX;
	double	translationY;
	double	translationZ;
} DisplayOutputTransformData;

@interface CocoaDSOutput : CocoaDSThread
{
	NSMutableDictionary *property;
	
	pthread_mutex_t *mutexConsume;
	pthread_rwlock_t *rwlockProducer;
}

@property (readonly) NSMutableDictionary *property;
@property (assign) pthread_rwlock_t *rwlockProducer;
@property (readonly) pthread_mutex_t *mutexConsume;

- (void) doCoreEmuFrame;
- (void) handleEmuFrameProcessed;

@end

@interface CocoaDSSpeaker : CocoaDSOutput
{
	NSUInteger bufferSize;
	
	OSSpinLock spinlockAudioOutputEngine;
	OSSpinLock spinlockVolume;
	OSSpinLock spinlockSpuAdvancedLogic;
	OSSpinLock spinlockSpuInterpolationMode;
	OSSpinLock spinlockSpuSyncMode;
	OSSpinLock spinlockSpuSyncMethod;
}

@property (assign) NSUInteger bufferSize;

- (id) init;
- (id) initWithVolume:(CGFloat)vol;
- (void) dealloc;

- (void) setVolume:(float)vol;
- (float) volume;
- (void) setAudioOutputEngine:(NSInteger)methodID;
- (NSInteger) audioOutputEngine;
- (void) setSpuAdvancedLogic:(BOOL)state;
- (BOOL) spuAdvancedLogic;
- (void) setSpuInterpolationMode:(NSInteger)modeID;
- (NSInteger) spuInterpolationMode;
- (void) setSpuSyncMode:(NSInteger)modeID;
- (NSInteger) spuSyncMode;
- (void) setSpuSyncMethod:(NSInteger)methodID;
- (NSInteger) spuSyncMethod;

- (BOOL) mute;
- (void) setMute:(BOOL)mute;
- (NSInteger) filter;
- (void) setFilter:(NSInteger)filter;
- (NSString *) audioOutputEngineString;
- (NSString *) spuInterpolationModeString;
- (NSString *) spuSyncMethodString;
- (void) handleSetVolume:(NSData *)volumeData;
- (void) handleSetAudioOutputEngine:(NSData *)methodIdData;
- (void) handleSetSpuAdvancedLogic:(NSData *)stateData;
- (void) handleSetSpuSyncMode:(NSData *)modeIdData;
- (void) handleSetSpuSyncMethod:(NSData *)methodIdData;
- (void) handleSetSpuInterpolationMode:(NSData *)modeIdData;

@end

@class CocoaVideoFilter;

@protocol CocoaDSDisplayDelegate <NSObject>

@required
- (void) doDisplayModeChanged:(NSInteger)displayModeID;

@optional
- (void) doDisplaySizeChanged:(NSSize)displaySize;

@end

@protocol CocoaDSDisplayVideoDelegate <CocoaDSDisplayDelegate>

@required
- (void) doInitVideoOutput:(NSDictionary *)properties;
- (void) doLoadVideoFrameWithMainBuffer:(const void *)mainBuffer
							touchBuffer:(const void *)touchBuffer
							  mainWidth:(const NSInteger)mainWidth
							 mainHeight:(const NSInteger)mainHeight
							 touchWidth:(const NSInteger)touchWidth
							touchHeight:(const NSInteger)touchHeight;
- (void) doProcessVideoFrameWithInfo:(const NDSFrameInfo &)frameInfo;

@optional
- (void) doResizeView:(NSRect)rect;
- (void) doTransformView:(const DisplayOutputTransformData *)transformData;
- (void) doRedraw;
- (void) doDisplayOrientationChanged:(NSInteger)displayOrientationID;
- (void) doDisplayOrderChanged:(NSInteger)displayOrderID;
- (void) doDisplayGapChanged:(float)displayGapScalar;

@end


@interface CocoaDSDisplay : CocoaDSOutput
{
	id <CocoaDSDisplayDelegate> delegate;
	NSSize displaySize;
	NSInteger displayMode;
	size_t _gpuCurrentWidth;
	size_t _gpuCurrentHeight;
	
	uint32_t _receivedFrameIndex;
	uint32_t _currentReceivedFrameIndex;
	uint32_t _receivedFrameCount;
	
	uint32_t _cpuLoadAvgARM9;
	uint32_t _cpuLoadAvgARM7;
	
	OSSpinLock spinlockDisplayType;
	OSSpinLock spinlockReceivedFrameIndex;
	OSSpinLock spinlockCPULoadAverage;
}

@property (retain) id <CocoaDSDisplayDelegate> delegate;
@property (readonly) NSSize displaySize;
@property (assign) NSInteger displayMode;

- (void) doReceiveGPUFrame;
- (void) handleReceiveGPUFrame;
- (void) handleChangeDisplayMode:(NSData *)displayModeData;
- (void) handleRequestScreenshot:(NSData *)fileURLStringData fileTypeData:(NSData *)fileTypeData;
- (void) handleCopyToPasteboard;

- (void) takeFrameCount;
- (void) setCPULoadAvgARM9:(uint32_t)loadAvgARM9 ARM7:(uint32_t)loadAvgARM7;
- (NSImage *) image;
- (NSBitmapImageRep *) bitmapImageRep;

@end

@interface CocoaDSDisplayVideo : CocoaDSDisplay
{
	
}

- (void) handleReceiveGPUFrame;
- (void) handleResizeView:(NSData *)rectData;
- (void) handleTransformView:(NSData *)transformData;
- (void) handleRedrawView;
- (void) handleReloadAndRedraw;
- (void) handleReprocessAndRedraw;
- (void) handleChangeDisplayOrientation:(NSData *)displayOrientationIdData;
- (void) handleChangeDisplayOrder:(NSData *)displayOrderIdData;
- (void) handleChangeDisplayGap:(NSData *)displayGapScalarData;

@end
