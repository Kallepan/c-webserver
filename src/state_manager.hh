#ifndef STATE_MANAGER_HH

#define STATE_MANAGER_HH
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

const int CLEANUP_INTERVAL = 10;

class StateManager {
  /**
   * This class is responsible for tracking the futures and periodically
   * cleaning up the completed futures from the vector
   */
public:
  StateManager()
      : stopCleanup(false), cleanUpThread(&StateManager::cleanUpLoop, this) {}

  ~StateManager() {
    stopCleanup = true;

    cv.notify_one();
    if (cleanUpThread.joinable())
      cleanUpThread.join();

    for (auto &future : futures) {
      try {
        future.wait();
      } catch (const std::exception &e) {
        std::cerr << "Exception in StateManager destructor: " << e.what()
                  << " Continuing...\n";
      }
    }
  }

  void addFuture(std::future<void> &&future) {
    std::lock_guard<std::mutex> lock(futures_mutex);
    futures.push_back(std::move(future));
    cv.notify_one();
  }

private:
  void cleanUpLoop() {
    /*
     * This function is responsible for periodically cleaning up the completed
     * futures from the vector
     * The function uses a condition variable to wait for the notification
     * from the addFuture function
     * The function uses the stopCleanup flag to stop the cleanup thread
     */
    while (!stopCleanup) {
      std::unique_lock<std::mutex> lock(futures_mutex);
      cv.wait(lock, [this] { return stopCleanup || !futures.empty(); });
      cleanUp();
      lock.unlock();
      std::this_thread::sleep_for(std::chrono::seconds(CLEANUP_INTERVAL));
    }
  }

  void cleanUp() {
    /**
     * This function is responsible for cleaning up the completed futures from
     * the vector
     * The function uses the erase-remove idiom to remove the completed futures
     * from the vector
     * The function uses the wait_for method to check if the future is ready
     * and then removes it from the vector
     *
     */
    futures.erase(std::remove_if(futures.begin(), futures.end(),
                                 [](const std::future<void> &future) {
                                   return future.wait_for(std::chrono::seconds(
                                              0)) == std::future_status::ready;
                                 }),
                  futures.end());
  }

  // vector to store the futures
  std::vector<std::future<void>> futures;

  // mutex to protect the futures vector
  std::mutex futures_mutex;

  // condition variable to notify the cleanup thread
  std::condition_variable cv;

  // cleanup thread control
  std::thread cleanUpThread;

  // flag to stop the cleanup thread
  std::atomic<bool> stopCleanup;
};
#endif