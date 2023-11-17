package com.ninevastudios.unrealfirebase;

import androidx.annotation.NonNull;

import com.google.android.gms.tasks.Continuation;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.functions.FirebaseFunctions;
import com.google.firebase.functions.HttpsCallableResult;

import java.util.List;
import java.util.Map;

@SuppressWarnings("unused")
public class FGCloudFunctions {

	public static native void OnStringFunctionComplete(long callbackAddr, String result);

	public static native void OnFunctionError(long callbackAddr, String error);

	public static native void OnIntFunctionComplete(long callbackAddr, int result);

	public static native void OnFloatFunctionComplete(long callbackAddr, float result);

	public static native void OnBoolFunctionComplete(long callbackAddr, boolean result);

	public static native void OnVoidFunctionComplete(long callbackAddr);

	public static native void OnMapFunctionComplete(long callbackAddr, Object result);

	public static native void OnArrayFunctionComplete(long callbackAddr, Object result);

	public static void callStringFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, String>() {
					@Override
					public String then(@NonNull Task<HttpsCallableResult> task) {
						return (String) task.getResult().getData();
					}
				})
				.addOnCompleteListener(new OnCompleteListener<String>() {
					@Override
					public void onComplete(@NonNull Task<String> task) {
						if (!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "String cloud function. An error occurred: " + e);
							return;
						}

						OnStringFunctionComplete(callback, task.getResult());
					}
				});
	}

	public static void callIntFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
	    FirebaseFunctions.getInstance(region)
                .getHttpsCallable(methodName)
                .call(parameters)
                .continueWith(new Continuation<HttpsCallableResult, Integer>() {
                    @Override
                    public Integer then (@NonNull Task<HttpsCallableResult> task) {
                        return (Integer) task.getResult().getData();
                    }
                })
                .addOnCompleteListener(new OnCompleteListener<Integer>() {
                    @Override
                    public void onComplete(@NonNull Task<Integer> task) {
                        if(!task.isSuccessful()) {
                            Exception e = task.getException();
                            OnFunctionError(callback, "Int cloud function. An error occurred: " + e);
                            return;
                        }

                        OnIntFunctionComplete(callback, task.getResult());
                    }
                });
    }

	public static void callFloatFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, Double>() {
					@Override
					public Double then (@NonNull Task<HttpsCallableResult> task) {
						return (Double) task.getResult().getData();
					}
				})
				.addOnCompleteListener(new OnCompleteListener<Double>() {
					@Override
					public void onComplete(@NonNull Task<Double> task) {
						if(!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "Float cloud function. An error occurred: " + e);
							return;
						}

						OnFloatFunctionComplete(callback, task.getResult().floatValue());
					}
				});
	}

	public static void callBoolFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, Boolean>() {
					@Override
					public Boolean then (@NonNull Task<HttpsCallableResult> task) {
						return (Boolean) task.getResult().getData();
					}
				})
				.addOnCompleteListener(new OnCompleteListener<Boolean>() {
					@Override
					public void onComplete(@NonNull Task<Boolean> task) {
						if(!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "Bool cloud function. An error occurred: " + e);
							return;
						}

						OnBoolFunctionComplete(callback, task.getResult());
					}
				});
	}

	public static void callVoidFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, Void>() {
					@Override
					public Void then (@NonNull Task<HttpsCallableResult> task) {
						return null;
					}
				})
				.addOnCompleteListener(new OnCompleteListener<Void>() {
					@Override
					public void onComplete(@NonNull Task<Void> task) {
						if(!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "Void cloud function. An error occurred: " + e);
							return;
						}

						OnVoidFunctionComplete(callback);
					}
				});
	}

	public static void callMapFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, Object>() {
					@Override
					public Object then (@NonNull Task<HttpsCallableResult> task) {
						return task.getResult().getData();
					}
				})
				.addOnCompleteListener(new OnCompleteListener<Object>() {
					@Override
					public void onComplete(@NonNull Task<Object> task) {
						if(!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "Map cloud function. An error occurred: " + e);
							return;
						}

						OnMapFunctionComplete(callback, task.getResult());
					}
				});
	}

	public static void callArrayFunction(final long callback, String methodName, String region, Map<String, Object> parameters) {
		FirebaseFunctions.getInstance(region)
				.getHttpsCallable(methodName)
				.call(parameters)
				.continueWith(new Continuation<HttpsCallableResult, Object>() {
					@Override
					public Object then (@NonNull Task<HttpsCallableResult> task) {
						return task.getResult().getData();
					}
				})
				.addOnCompleteListener(new OnCompleteListener<Object>() {
					@Override
					public void onComplete(@NonNull Task<Object> task) {
						if(!task.isSuccessful()) {
							Exception e = task.getException();
							OnFunctionError(callback, "Array cloud function. An error occurred: " + e);
							return;
						}

						OnArrayFunctionComplete(callback, task.getResult());
					}
				});
	}
}